#include "loader.h"

void print_section(Section* sec){
string type="";
if(sec->type == Section::SectionType::SEC_TYPE_CODE){type = "SEC_CODE_TYPE";}
else{type="SEC_TYPE_DATA";}

printf("\n::: SECTION_INFO :::\nBinary_loaded: %s      SECTION_TYPE : %s       START_ADDRESS : %016x      SIZE: %lu\n",sec->binary->filename.c_str(),type.c_str(),sec->vma,sec->size);	
printf("\n::: %s SECTION :::\n\n",sec->name.c_str());

printf("%016x: ",sec->vma);
for(int i=1;i<=sec->size;i++){printf("%02x",sec->bytes[i-1]);if(i%16==0&&i!=sec->size){printf("\n%016x: ",sec->vma+i);}else if(i%4==0&&i!=sec->size){printf("  ");}} 
printf("\n");

                                }





int main(int argc , char* argv[]){
	

if(argc <2||argc == 3||argc>4){
printf("Usage: '%s' binaryfile <options> SectionName \n",argv[0]);
printf("<options>:\n -S %-7s show section content in hexadecimal\n"," ");
return 0;
}

string name = string(argv[1]);
Binary  bin = Binary();
if(!load_binary(name,&bin)){return 0;}


if(argc == 2){

printf("binary=> '%s' | format: %s | architecture: %s | (%u bits) | Entry_Address@0x%016x \n\n",bin.filename.c_str(),bin.type_str.c_str(),bin.arch_str.c_str(),bin.bits,bin.entry);


printf("::: SYMBOL TABLE :::\n\n");
printf("%*s %-80s %s    %-15s%s\n",7,"","SYMBOL_NAME","SYMBOL_ADDRESS","SYMBOL_TYPE","SYMBOL_BIND");

for(int i =0;i<bin.symbols.size();i++){

Symbol* sym = &bin.symbols[i];
string type= "TYPE_UNKNOWN";
string bind = "UNKNOWN";

if(sym->type==Symbol::SYM_TYPE_FUNC){type = "TYPE_FUNCTION";}
else if(sym->type ==Symbol::SYM_TYPE_DATA){type="TYPE_DATA";}

if(sym->bind == Symbol::Bindtype::GLOBAL){bind ="GLOBAL";}
else if(sym->bind == Symbol::Bindtype::LOCAL){bind ="LOCAL";}
else if(sym->bind == Symbol::Bindtype::WEAK){bind ="WEAK";}
printf("%5i: %-80s %016x   %-15s %s\n",i,sym->name.c_str(),sym->addr,type.c_str(),bind.c_str());
                                      
                                     }


printf("\n::: SECTION TABLE :::\n\n");
printf("%*s %-80s %s    %-15s%s\n",7,"","SECTION_NAME","SECTION_ADDRESS","SECTION_TYPE","SIZE");

for(int i=0;i<bin.sections.size();i++){

Section* section = &bin.sections[i];
string type ="SEC_TYPE_NONE";

if(section->type == Section::SectionType::SEC_TYPE_CODE){type ="SEC_TYPE_CODE";}
else if(section->type == Section::SectionType::SEC_TYPE_DATA){type ="SEC_TYPE_DATA";}
printf("%5i: %-80s %016x   %-15s %lu\n",i,section->name.c_str(),section->vma,type.c_str(),section->size);


                                    }

}


if(argc == 4){

if(string(argv[2])=="-S"){

string name = string(argv[3]);
for(int i=0;i<bin.sections.size();i++){if(bin.sections[i].name == name){ print_section(&bin.sections[i]); unload_binary(&bin); return 0;}}
printf("No Section with name '%s'\n",name.c_str());

                         }


else{printf("'%s' is wrong option !!\n",argv[2]);}


           }


unload_binary(&bin);


return 0;
}
