#ifndef MACROS_H
# define MACROS_H

# define FALSE      0
# define TRUE       1
# define END        0

# define PADDING_LEN_32                          11
# define PADDING_LEN_64                          19
# define SYMBOL_PADDING                          3

# define HEX_DIGITS                              "0123456789abcdef"
# define UNDEFINED_SYMBOL '?'

# define SYMBOL_IS_LOCAL(elf)   (ELF64_ST_BIND(elf.symbol_table->st_info) == STB_LOCAL)
# define SYMBOL_NAME(elf)       (elf.strtab+elf.symbol_table->st_name)
# define SYMBOL_ADRESS(elf)     (elf.symbol_table->st_value)
# define SECTION_HEADER(elf)    (elf.symbol_table->st_shndx <= elf.header->e_shnum ? &elf.section_hdr[elf.symbol_table->st_shndx] : NULL)
# define SH_FLAG(section_hdr)   (section_hdr->sh_flags)
# define SH_INDEX(symbol_table) (symbol_table->st_shndx)
# define SH_TYPE(symbol_table)  (section_hdr->sh_type )

# define IS_LOCAL(c) (c >= 'a' && c <= 'z' && symbol != 'v' && symbol != 'w' && symbol != 'i') ? true : false

#endif