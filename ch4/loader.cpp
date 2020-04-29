#include "loader.h"
#include <stdio.h>

bfd* OpenBinary(string &name){

static bool inited = false;
if(!inited){bfd_init(); inited= true;}

bfd* fHandle = bfd_openr(name.c_str(),NULL);
if(!fHandle){ fprintf(stderr, "'%s' failed to be opened ( %s ) \n",name.c_str(),bfd_errmsg(bfd_get_error())); return NULL; }

if(!bfd_check_format(fHandle,bfd_object)){
fprintf(stderr,"'%s' file is not executable ( %s ) \n",name.c_str(),bfd_errmsg(bfd_get_error())); return NULL;
                                         }

bfd_set_error(bfd_error_no_error);

if(bfd_get_flavour(fHandle)==bfd_target_unknown_flavour){
fprintf(stderr,"'%s' file has unknown format ( %s ) \n",name.c_str(),bfd_errmsg(bfd_get_error())); return NULL;
                                                        }
return fHandle;
                            } 




static int load_stsym(bfd* handle,Binary* bin){

long size = bfd_get_symtab_upper_bound(handle);
if(size <0){fprintf(stderr,"failed to read static symbol table with error messaeg %s\n",bfd_errmsg(bfd_get_error()));  return -1;}
asymbol **symbol_table = (asymbol**)malloc(size);

if(!symbol_table){fprintf(stderr,"out of memory"); return -1;}
int num_symbols =  bfd_canonicalize_symtab(handle,symbol_table);
if(num_symbols <0 ){fprintf(stderr,"failed to read static symbol table with error %s\n",bfd_errmsg(bfd_get_error()));free(symbol_table); return -1;}


for(int i =0;i<num_symbols;i++){

if(symbol_table[i]->flags & BSF_FUNCTION){
Symbol sym = Symbol();
if(symbol_table[i]->flags & BSF_WEAK){sym.bind=Symbol::Bindtype::WEAK;}
else if(symbol_table[i]->flags & BSF_GLOBAL){sym.bind=Symbol::Bindtype::GLOBAL;}
else if(symbol_table[i]->flags & BSF_LOCAL){sym.bind=Symbol::Bindtype::LOCAL;}
else{sym.bind = Symbol::Bindtype::UNKNOWN;}
sym.name= string(symbol_table[i]->name);
sym.type= Symbol::SymbolType::SYM_TYPE_FUNC;
sym.addr= bfd_asymbol_value(symbol_table[i]);
bin->symbols.push_back(sym);

                                            }

else if( symbol_table[i]->flags & BSF_OBJECT){
Symbol sym = Symbol();
bool check =false;
if(symbol_table[i]->flags & BSF_WEAK){sym.bind=Symbol::Bindtype::WEAK;check=true;}
else if(symbol_table[i]->flags & BSF_GLOBAL){sym.bind=Symbol::Bindtype::GLOBAL;check=true;}
else if(symbol_table[i]->flags & BSF_LOCAL){sym.bind=Symbol::Bindtype::LOCAL;check=true;}
if(check){
sym.name= string(symbol_table[i]->name);
sym.type= Symbol::SymbolType::SYM_TYPE_DATA;
sym.addr= bfd_asymbol_value(symbol_table[i]);
bin->symbols.push_back(sym);}                

                                            }
}


free(symbol_table);
return num_symbols;
}


static int load_dynsym(bfd* handle ,Binary* bin){

long size = bfd_get_dynamic_symtab_upper_bound(handle);
if(size <0){fprintf(stderr,"failed to read dynamic symbol table with error messaeg %s\n",bfd_errmsg(bfd_get_error()));  return -1;}
asymbol **symbol_table = (asymbol**)malloc(size);

if(!symbol_table){fprintf(stderr,"out of memory"); return -1;}

int num_symbols =  bfd_canonicalize_dynamic_symtab(handle,symbol_table);

if(num_symbols <0 ){fprintf(stderr,"failed to read dynamic symbol table with error %s\n",bfd_errmsg(bfd_get_error()));free(symbol_table); return -1;}



for(int i =0;i<num_symbols;i++){
if(symbol_table[i]->flags & BSF_FUNCTION){
Symbol sym = Symbol();
if(symbol_table[i]->flags & BSF_WEAK){sym.bind=Symbol::Bindtype::WEAK;}
else if(symbol_table[i]->flags & BSF_GLOBAL){sym.bind=Symbol::Bindtype::GLOBAL;}
else if(symbol_table[i]->flags & BSF_LOCAL){sym.bind=Symbol::Bindtype::LOCAL;}
else{sym.bind=Symbol::Bindtype::UNKNOWN;}
sym.name= string(symbol_table[i]->name);
sym.type= Symbol::SymbolType::SYM_TYPE_FUNC;
sym.addr= bfd_asymbol_value(symbol_table[i]);
bin->symbols.push_back(sym);
                                            }


else if(symbol_table[i]->flags & BSF_OBJECT){
bool check = false;
Symbol sym = Symbol();
if(symbol_table[i]->flags & BSF_WEAK){sym.bind=Symbol::Bindtype::WEAK; check = true;}
else if(symbol_table[i]->flags & BSF_GLOBAL){sym.bind=Symbol::Bindtype::GLOBAL; check = true;}
else if(symbol_table[i]->flags & BSF_LOCAL){sym.bind=Symbol::Bindtype::LOCAL;check=true;}
if(check){
sym.name= string(symbol_table[i]->name);
sym.type= Symbol::SymbolType::SYM_TYPE_DATA;
sym.addr= bfd_asymbol_value(symbol_table[i]);
bin->symbols.push_back(sym);}
                                          }
}


free(symbol_table);
return num_symbols;
}

static int load_sections(bfd* handle, Binary* bin){
int bfd_flags;
const char* secname;
uint64_t vma,size;
asection* section = handle->sections;

while(section){

Section sec= Section();
sec.binary=bin;
secname = bfd_section_name(section);

if(!secname){sec.name="<unnamed>";}
else{sec.name= string(secname);}

vma= bfd_section_vma(section);
size= bfd_section_size(section);
sec.size = size;
sec.vma = vma;

bfd_flags = bfd_section_flags(section);
if(bfd_flags & SEC_CODE ){sec.type= Section::SectionType::SEC_TYPE_CODE;}
else if(bfd_flags * SEC_DATA){sec.type = Section::SectionType::SEC_TYPE_DATA;}
else{continue;}

sec.bytes =(uint8_t*) malloc(size);
if(!sec.bytes){fprintf(stderr,"out of memory\n"); return -1;}

if(!bfd_get_section_contents(handle,section,sec.bytes,0,size)){

fprintf(stderr,"failed to read '%s' section with error ( %s )\n", secname,bfd_errmsg(bfd_get_error()));

return -1;	
}

bin->sections.push_back(sec);
section= section->next;

}

return 0;
}






static bool load_binary_bfd(string &name , Binary *bin){

bfd* handle = OpenBinary(name);
if(!handle){return false;}


bin->filename = name;
bin->entry = handle->start_address;
bfd_flavour flav = bfd_get_flavour(handle);
bin->type_str = bfd_get_target(handle);

if(flav ==bfd_target_coff_flavour){bin->type=Binary::BinaryType::BIN_TYPE_PE;}
else if(flav == bfd_target_elf_flavour){bin->type=Binary::BinaryType::BIN_TYPE_ELF;}
else{
fprintf(stderr,"binary file '%s' has unsupported format ( %s ) \n",name.c_str(),bfd_flavour_name(flav)); bfd_close(handle); return false;
    }

const bfd_arch_info_type* arch = bfd_get_arch_info(handle);
bin->arch_str = string (arch->printable_name);

if(arch->mach == bfd_mach_i386_i386 ){bin->arch = Binary::BinaryArch::ARCH_X86; bin->bits=32;}
else if(arch->mach ==bfd_mach_x86_64){bin->arch = Binary::BinaryArch::ARCH_X86; bin->bits=64;}
else{
fprintf(stderr,"binary file '%s' has unsupported architecture ( %s ) \n",name.c_str(),arch->printable_name); bfd_close(handle); return false;
    }


// load static sym
if(load_stsym(handle , bin)== -1){ bfd_close(handle);return false;}

// load dynamic sym

if(load_dynsym(handle,bin)== -1){ bfd_close(handle);return false;}
// load sections
if(load_sections(handle,bin)==-1){bfd_close(handle);return false;}

bfd_close(handle);
return true;
}

void unload_binary(Binary* bin){

Section * sec;

for(int i=0;i<bin->sections.size();i++){
sec = &bin->sections[i];
if(sec->bytes){free(sec->bytes);}

                                      }
}



bool load_binary(string & name, Binary* bin){

return load_binary_bfd(name,bin);
}

