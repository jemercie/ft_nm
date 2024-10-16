
<h2 style="text-align:center">nm</h2>

_name mangling_
> GNU nm lists the symbols from object files objfile.... If no object files are listed as arguments, nm assumes the file a.out. 

**use:**
    nm \[*options*\] \[*objfile...\]

### 3 colonnes:

    |  virtual adress  | symbol type |        symbol name       |
    -------------------------------------------------------------
    | 000000000040037c |      r      |        __abi_tag         |
    | 0000000000407208 |      B      |       __bss_start        |
    | 00000000004031d0 |      T      |    check_count_option    |
    | 0000000000401730 |      t      | check_flood_and_interval |
 

### types of symbol:

_Lower case is **Local** symbol_
_Uppercase is **External** symbol_


    A           Absolute symbol, will not be changed by further linking
    B - b       In the uninitialized data section (known as BSS)
    C           Common symbols, uninitialized data. When linking, 
                multiple common symbols may appear with the same name. If 
                the symbol is defined anywhere, the common symbols 
                are treated as undefined references.
    D -  d      In the initialized data section
    G - g       Initialized data section for small objects. Some object 
                file formats permit more efficient access to small data 
                objects, such as a global int variable as opposed to a 
                large global array.
    i           For PE format files this indicates that the symbol is  
                in a section specific to the implementation of DLLs
    N           Debugging symbol
    p           The symbols is in a stack unwind section.
    R - r       The symbol is in a read only data section.
    S - s       The symbol is in an uninitialized data section for small objects
    T - t       The symbol is in the text (code) section.
    U           Undefined symbol right now
    V - v       The symbol is a weak object. When a weak defined symbol is linked 
                with a normal defined symbol, the normal defined symbol is used with
                no error. When a weak undefined symbol is linked and the symbol is 
                not defined, the value of the weak symbol becomes zero with no 
                error. On some systems, uppercase indicates that a default value has
                been specified.
    W - w       The symbol is a weak symbol that has not been specifically tagged as 
                a weak object symbol. When a weak defined symbol is linked with a 
                normal defined symbol, the normal defined symbol is used with no 
                error. When a weak undefined symbol is linked and the symbol is not 
                defined, the value of the symbol is determined in a system-specific 
                manner without error. On some systems, uppercase indicates that a 
                default value has been specified.
    - X         Affiche la valeur et la taille d'un symbole sous la forme d'un nombre 
                hexadécimal plutôt que d'un nombre décimal.
    -           The symbol is a stabs symbol in an a.out object file. In this case,
                the next values printed are the stabs other field, the stabs desc 
                field, and the stab type. Stabs symbols are used to hold debugging 
                information.
    ?           The symbol type is unknown, or object file format specific.



options cool:
    -A
    -n (display in sorted order)
    -g only external symbols (uppercase)
    -S display size

#### _"You have to work with ELF binaries. You have to handle x86_32, x64, object files, .so"_

**ELF** _(Executable Linkable Format)_ is a common standard file format for executable files, object code, shared libraries, and core dumps.

## Segments and sections: _they represent data from the ELF file_

### Segments
segments are exclusively used at runtime

they points to an absolute address in the memory and specify how many bytes are contained in the segment. Some segment types haven't any content

segments have instructions that tells the operating system how to map part 
of the ELF file into memory
    
in a dinamically linked binary we have an additionnal segment in the ELF file that specifies 
which shared library need to be loaded.



### Sections
    
    sections are almost only exclusively used at link time

    in most cases, sections and segments overlap

## ELF header

The ELF header defines whether to use _32-bit_ or _64-bit_ addresses. The header contains three fields that are affected by this setting and offset other fields that follow them. The ELF header is **52 bytes long for 32-bit binaries** or **64 bytes long for 64-bit binaries.**

structure contenant les metadata du fichier

```typedef struct elf64_hdr {
  unsigned char e_ident[EI_NIDENT];  /* explain how the elf file need to be parsed */
  Elf64_Half    e_type;              /* specifies the type of the ELF file */
  Elf64_Half    e_machine;           /* always set to one */
  Elf64_Word    e_version;
  Elf64_Addr    e_entry;             /* Entry point virtual address to our executable, 0 if None */
  Elf64_Off     e_phoff;             /* Program header table file offset */
  Elf64_Off     e_shoff;             /* Section header table file offset */
  Elf64_Word    e_flags;
  Elf64_Half    e_ehsize;
  Elf64_Half    e_phentsize;
  Elf64_Half    e_phnum;
  Elf64_Half    e_shentsize;
  Elf64_Half    e_shnum;
  Elf64_Half    e_shstrndx;           /* section header string table content */
} Elf64_Ehdr;
```

###         e_ident
```
            /* magic number, the same for all elf files  hex 7f and E, L, F encoded as ascii */
#define        EI_MAG0                0 // 7f
#define        EI_MAG1                1 // 45 (E)
#define        EI_MAG2                2 // 4c (L)
#define        EI_MAG3                3 // 46 (F)
            /* class of the elf file (0 = None, 1 = 32 bit objects, 2 = 64 bits objects) */
#define        EI_CLASS               4
            /* data encoding used in the file (endianness) (0 = None, 1 = LSB(Least significant byte first, MSB(Most SB))) */
#define        EI_DATA                5
#define        EI_VERSION             6 // always 1
            /* Operating System Application Binary Interface os abi (souvent 0)*/
#define        EI_OSABI               7
#define        EI_PAD                 8 // padding
```

###         e_type
```
0x00 	ET_NONE 	// Unknown.
0x01 	ET_REL 	    // Relocatable file (object files)
0x02 	ET_EXEC 	// Executable file doesn't support ASLR (Address space layout randomization), use the -no-pie
0x03 	ET_DYN 	    // Shared object like librairies
0x04 	ET_CORE 	// Core file
0xFE00 	ET_LOOS 	// Reserved inclusive range
0xFEFF 	ET_HIOS     // Operating system specific
0xFF00 	ET_LOPROC 	// Reserved inclusive range
0xFFFF 	ET_HIPROC   // Processor specific
```

