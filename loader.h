
#ifndef loaderH
#define loaderH

#define PACKAGE "bfdloader"
#define PACKAGE_VERSION "1.0"
#include <bfd.h>
#include <stdint.h>
#include <string>
#include <vector>
using namespace std;

class Binary;
class Symbol;
class Symbol {
public:
enum SymbolType {SYM_TYPE_UKN = 0,SYM_TYPE_FUNC = 1,SYM_TYPE_DATA = 2};
enum Bindtype{ UNKNOWN ,GLOBAL , LOCAL , WEAK};
Symbol() : type(SYM_TYPE_UKN),bind(UNKNOWN) ,name(), addr(0) {}
SymbolType type;
Bindtype bind;
string name;
uint64_t addr;
};


class Section {
public:
enum SectionType {SEC_TYPE_NONE = 0,SEC_TYPE_CODE = 1,SEC_TYPE_DATA = 2};
Section() : binary(NULL), type(SEC_TYPE_NONE),

vma(0), size(0), bytes(NULL) {}
bool contains(uint64_t addr) { return (addr >= vma) && (addr-vma < size); }
Binary *binary;
string name;
SectionType type;
uint64_t vma;
uint64_t size;
uint8_t *bytes;
};

class Binary {
public:
enum BinaryType {BIN_TYPE_AUTO = 0,BIN_TYPE_ELF = 1,BIN_TYPE_PE = 2};
enum BinaryArch { ARCH_NONE = 0,ARCH_X86 = 1};

Binary() : type(BIN_TYPE_AUTO), arch(ARCH_NONE), bits(0), entry(0) {}

Section *get_text_section(){ for(auto &s : sections) if(s.name == ".text") return &s; return NULL; }
string filename;
BinaryType type;
string type_str;
BinaryArch arch;
string arch_str;
unsigned bits;
uint64_t entry;
vector<Section> sections;
vector<Symbol> symbols;
};

static bfd* OpenBinary(string&);
bool load_binary(string& name,Binary* bin);
static bool load_binary_bfd(string& , Binary*);
void unload_binary(Binary*);
static int Load_Stsym(bfd*,Binary*);
static int load_dynsym(bfd*,Binary*);
static int load_sections(bfd*, Binary*);
void unload_binary(Binary*);

#endif

