
# ft_nm

"This project is about recoding the command nm"

 `nm`is a unix cmd used to **dump the symbol table** and their attributes from a **ELF** binary executable file.This is a *binary tool* part of the [GNU binutils](https://www.gnu.org/software/binutils/) package.

## Usage

```
Usage: nm [option(s)] [file(s)]
 List symbols in [file(s)] (a.out by default).
 The options are:
  -g, --extern-only      Display only external symbols
  -p, --no-sort          Do not sort the symbols
  -r, --reverse-sort     Reverse the sense of the sort
  -u, --undefined-only   Display only undefined symbols
  -?, --help             Display this information
nm: supported targets: x86_32, x64, object files, .so
``` 
    
## Output

    |  virtual adress  | symbol type |        symbol name       |
    -------------------------------------------------------------
    | 000000000040037c |      r      |        __abi_tag         |
    | 0000000000407208 |      B      |       __bss_start        |
    | 00000000004031d0 |      T      |    check_count_option    |
    | 0000000000401730 |      t      | check_flood_and_interval |

## How

**ELF**, formerly named **E**xtensible **L**inking **F**ormat is a common standard file format for *executable files*, 
*object code*, *shared libraries*, and *core dumps*. 

Each symbol represents a **entity or data type**, such as a *function*, a *global variable*, or a *static variable*.

The ELF file begins with a header that provides offsets to sections, including the section header table and the string table (which stores names for sections and symbols). To locate the symbol table, you iterate through the section header table until finding a section of type `SYMTAB`, which contains the symbols. 

From the symbol table, each entry provides details about the symbol, such as its name (an index into the string table), type (e.g., function, variable), and other attributes. 

By referencing the string table via these indices, the actual names of the symbols can be resolved. Depending on the symbol type and flags, you can identify its kind, such as functions (T or t), global variables (D or d), static variables (B or b), or undefined symbols (U).


Done reading [this doc](https://docs.oracle.com/cd/E19683-01/816-1386/6m7qcoblj/index.html#chapter7-27) with structs at [elf.h](https://sites.uclouvain.be/SystInfo/usr/include/linux/elf.h.html) and watching a [really nice video about ELF](https://www.youtube.com/watch?v=nC1U1LJQL8o)

Useful cmd: `readelf -g --section-details --symbols -X`

### Types of symbol:

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

**ELF** _(Executable Linkable Format)_ is a common standard file format for executable files, object code, shared libraries, and core dumps.


## ELF header

> readelf -h _binary_

The ELF header defines whether to use _32-bit_ or _64-bit_ addresses. The header contains three fields that are affected by this setting and offset other fields that follow them. The ELF header is **52 bytes long for 32-bit binaries** or **64 bytes long for 64-bit binaries.**

Struct  **Elf64/32_Ehdr** containing file's metadata
```
typedef struct elf64_hdr {
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
####         e_ident values
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
####         e_type values
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
### Segments and sections: _they represent data from the ELF file_
#### Segments
> readelf --segments _binary_
>
Segments are exclusively used at runtime
They points to an absolute address in the memory and specify how many bytes are contained in the segment. Some segment types haven't any content
In a dinamically linked binary we have an additionnal segment in the ELF file that specifies
which shared library need to be loaded.
Segments have instructions that tells the operating system how to map part
of the ELF file into memory.
**Each segment is defined by a small header structure (Program Header)**
all those program headers are in an array in the ELF file, and the location of the
Program Header Table is specified in the ELF header (**e_phoff**), it also
specifies the size of each program header entry (**e_phentsize**) and the number of
entries (**e_phnum**)
#### Program header
_Describing zero or more memory segments_
>
```
typedef struct elf64_phdr {
Elf64_Word p_type;
Elf64_Word p_flags;
Elf64_Off p_offset;                /* Segment file offset */
Elf64_Addr p_vaddr;                /* Segment virtual address */
Elf64_Addr p_paddr;                /* Segment physical address */
Elf64_Xword p_filesz;                /* Segment size in file */
Elf64_Xword p_memsz;                /* Segment size in memory */
Elf64_Xword p_align;                /* Segment alignment, file & memory */
} Elf64_Phdr;
```

### p_type
Specifies the type of the associated segment.

**_Values:_**
- **_PT_NULL_**: A placeholder. Setting the value type to PT_NULL is an easy way to disable a segment.
- **_PT_LOAD_**: Segment type that will be loaded in memory (how is specified by other members of the program header). It can also be used to create zero-initialized segments by specifying a larger size in memory.
- **_PT_DYNAMIC_**: Segment type that contains information required for dynamically linked binaries. It's interpreted by the operating system to load shared libraries related to an executable.
- **_PT_INTERP_**: Specifies the program interpreter required by dynamic executables. This is a string containing an absolute path in the filesystem. Only executables can have this segment type, shared libraries will not.
- **PT_NOTE**: Holds auxiliary information for the binary, such as for a debugger or other tools.
- **_PT_SHLIB_**: Shared library type; undefined and should never be used. ELF files with this section are not conformant with the ELF Specification.
- **_PT_PHDR_**: Specifies whether and where the program header table will be loaded in memory.
- **_PT_TLS_**: Thread Local Storage. Contains information about initialized thread-local variables.
- **PT_LOOS - PT_HIOS (0x60000000 - 0x6FFFFFFF)**: Operating system-specific segment types. For example, on GNU/Linux, this includes a segment that specifies which parts of memory should be marked as read-only after the dynamic loader completes its job.
- **PT_LOPROC - PT_HIPROC (0x70000000 - 0x7FFFFFFF)**: A reserved range for processor-specific segment types.

### p_flags
Specifies the permissions of the segment.

**_Values:_**
- **PF_X**: Executable
- **PF_W**: Writable
- **PF_R**: Readable

### p_offset
Specifies the location of the segment (if it has any) in the ELF file.

### v_addr
Specifies the virtual address where the first byte of the segment will be in memory.

### p_paddr
Used in contexts where physical addresses are relevant, such as firmware. It specifies the physical address where the segment will be loaded.

### p_filesz
Specifies the size of the segment in the file. If this is 0, the segment is defined exclusively by the program header and has no further content in the file.

### p_memsz
Specifies the size of the segment in memory. If this is larger than the size in the file, the leftover bytes will be initialized as zero.

### p_align
Specifies the alignment requirement for the segment.

---

## Sections

Defined by a *section header*, which is part of the *section header table*. The section header table is specified in the ELF header (`e_shoff`), and it also specifies the size of each program header entry (`e_shentsize`) and the number of entries (`e_shnum`).

Sections are used during linking and by tools like debuggers. During execution and reverse engineering, only the segments are considered.

You can use `strip` to remove all sections from an executable and use `readelf --sections`. It will print an error, but the executable will still run just fine.

### Section Header Structure

```c
typedef struct elf64_shdr {
  Elf64_Word sh_name;                /* Section name, index in string table */
  Elf64_Word sh_type;                /* Type of section */
  Elf64_Xword sh_flags;              /* Miscellaneous section attributes */
  Elf64_Addr sh_addr;                /* Section virtual address at execution */
  Elf64_Off sh_offset;               /* Section file offset */
  Elf64_Xword sh_size;               /* Size of section in bytes */
  Elf64_Word sh_link;                /* Index of another section */
  Elf64_Word sh_info;                /* Additional section information */
  Elf64_Xword sh_addralign;          /* Section alignment */
  Elf64_Xword sh_entsize;            /* Entry size if section holds table */
} Elf64_Shdr;

idk how to optimize flags(etc) checking, so i put them in kind of decressant occurence order


