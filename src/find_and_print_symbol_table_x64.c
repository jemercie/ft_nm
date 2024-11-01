#include "nm.h"
#include <elf.h>

static int  get_symtab_index(Elf64_Ehdr *elf_header, Elf64_Shdr *section_hdr_table);
static void init_elf_struct(t_file *file, t_elf64 *elf);

static char resolve_symbol_type(Elf64_Sym   *symbol_table, Elf64_Shdr *section_hdr){

    char c = 'K';

    if (ELF64_ST_TYPE(symbol_table->st_info) == STT_GNU_IFUNC)
        return 'i';
    else if ((ELF64_ST_TYPE(symbol_table->st_info) == STT_FILE))
        return 'a';
    else if (ELF64_ST_BIND(symbol_table->st_info) == STB_WEAK){
        if (ELF64_ST_TYPE(symbol_table->st_info) == STT_OBJECT)
            return symbol_table->st_shndx == SHN_UNDEF ? 'v' : 'V';
        return symbol_table->st_shndx == SHN_UNDEF ? 'w' : 'W';
    }
    else if ((section_hdr && (section_hdr->sh_type == SHT_PROGBITS)&& section_hdr->sh_flags == (SHF_ALLOC | SHF_EXECINSTR)))
        return 'T';
    else if (symbol_table->st_shndx == SHN_UNDEF)
        return 'U';
    else if ((section_hdr && section_hdr->sh_type == SHT_PROGBITS && section_hdr->sh_flags == (SHF_ALLOC) )|| (section_hdr && ( (section_hdr->sh_flags & SHF_STRINGS) > 0 || section_hdr->sh_type == SHT_REL || section_hdr->sh_type == SHT_RELA || (section_hdr && section_hdr->sh_type == SHT_NOTE))))
        return 'R';
    else if ((ELF64_ST_TYPE(symbol_table->st_info) == STT_OBJECT || ELF64_ST_TYPE(symbol_table->st_info) == STT_NOTYPE || ((section_hdr && (section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE) && (section_hdr->sh_type == SHT_INIT_ARRAY || section_hdr->sh_type == SHT_FINI_ARRAY || section_hdr->sh_type == SHT_DYNAMIC || section_hdr->sh_flags == SHT_SHLIB))) || section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS))))
        return section_hdr->sh_type == SHT_NOBITS ? 'B' : 'D';
    else if (ELF64_ST_BIND(symbol_table->st_info) == STB_GNU_UNIQUE)
        return'u';
    else if (symbol_table->st_shndx == SHN_ABS)
       return 'A';
    else if (symbol_table->st_shndx == SHN_COMMON)
        return 'C';
    else if ( c == 'K')
        return '?';

    return c;
}

static char resolve_undefined_symbol_type_only(Elf64_Sym   *symbol_table){

    if (ELF64_ST_BIND(symbol_table->st_info) == STB_WEAK){
        if (ELF64_ST_TYPE(symbol_table->st_info) == STT_OBJECT)
            return symbol_table->st_shndx == SHN_UNDEF ? 'v' : '?';
        return symbol_table->st_shndx == SHN_UNDEF ? 'w' : '?';
    }
    else if (symbol_table->st_shndx == SHN_UNDEF)
        return 'U';
    
    return '?';
}

bool find_and_print_symbol_table_x64(t_file *file, t_options *options){

    static t_elf64  elf;
    t_symbol        *lst = NULL;
    char            symbol;

    init_elf_struct(file, &elf);
    for (size_t j = 1; j < elf.symbols_nb; j++) {
        if (options->undefined_only){
            symbol = resolve_undefined_symbol_type_only(&elf.symbol_table[j]);
            if (symbol != '?')
                lst = add_symbol_to_lst(lst, elf.strtab+elf.symbol_table[j].st_name, symbol, elf.symbol_table[j].st_value, options);
        }
        else if ((ELF64_ST_TYPE(elf.symbol_table[j].st_info) == STT_FILE || \
            ft_strlen(elf.strtab+elf.symbol_table[j].st_name) == 0))
            if (options->debug_symbols){
                symbol = 'a';
                lst = add_symbol_to_lst(lst, elf.strtab+elf.symbol_table[j].st_name, symbol, elf.symbol_table[j].st_value, options);
            }
            else
                continue;
        else{
            symbol = resolve_symbol_type(&elf.symbol_table[j], elf.symbol_table[j].st_shndx \
                    <= elf.header->e_shnum ? &elf.section_hdr[elf.symbol_table[j].st_shndx] : NULL);
            
            if (ELF64_ST_BIND(elf.symbol_table[j].st_info) == STB_LOCAL && symbol != UNDEFINED_SYMBOL)
                symbol += 32;
            lst = add_symbol_to_lst(lst, elf.strtab+elf.symbol_table[j].st_name, symbol, elf.symbol_table[j].st_value, options);
        }
    }
    if (options->reverse_sort & !options->no_sort)
        recursive_print_lst(lst, PADDING_LEN_64);
    else
        print_lst(lst, PADDING_LEN_64);
    return TRUE;
}

static int get_symtab_index(Elf64_Ehdr *elf_header, Elf64_Shdr *section_hdr_table){

        for (int i = 0; i < elf_header->e_shnum; i++){
        if (section_hdr_table[i].sh_type == SHT_SYMTAB){
            return i;
        }
    }
    return -1;
}

#include <stdio.h>

static void init_elf_struct(t_file *file, t_elf64 *elf){

    ft_memset(elf, 0, sizeof(t_elf64));

    elf->header         = (Elf64_Ehdr *)file->file;
    elf->section_hdr    = (Elf64_Shdr *)&file->file[elf->header->e_shoff];
    elf->symtab_index   = get_symtab_index(elf->header, elf->section_hdr);

    if (elf->symtab_index == -1){
        PRINT_ERROR_NO_SYMBOL_AND_EXIT(file->filename);
    }

    elf->symbol_table   = (Elf64_Sym *)(file->file + elf->section_hdr[elf->symtab_index].sh_offset);
    elf->strtab_section = &elf->section_hdr[elf->section_hdr[elf->symtab_index].sh_link];
    elf->strtab         = (char *)(file->file + elf->strtab_section->sh_offset);
    elf->symbols_nb     = elf->section_hdr[elf->symtab_index].sh_size / sizeof(Elf64_Sym);

}