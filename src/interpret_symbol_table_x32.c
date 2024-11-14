#include "nm.h"

# define ST_TYPE(symbol_table)  (ELF32_ST_TYPE(symbol_table->st_info))
# define ST_BIND(symbol_table)  (ELF32_ST_BIND(symbol_table->st_info))
# define SYMBOL_TYPE(elf)       (ELF32_ST_TYPE(elf.symbol_table->st_info))

static char resolve_symbol_type(Elf32_Sym   *symbol_table, Elf32_Shdr *section_hdr);
static bool symbol_is_data_section(Elf32_Sym   *symbol_table, Elf32_Shdr *section_hdr);
static bool symbol_is_read_only_data_section(Elf32_Shdr *section_hdr);
static char resolve_undefined_symbol_type(Elf32_Sym   *symbol_table);
static void init_elf_struct(t_file *file, t_elf32 *elf);
static int  get_symtab_index(Elf32_Ehdr *elf_header, Elf32_Shdr *section_hdr_table);


bool interpret_symbol_table_x32(t_file *file, t_options *opt){

    static t_elf32  elf;
    char            symbol;
    t_symbol        *lst = NULL;

    init_elf_struct(file, &elf);

    for (size_t j = 1; j < elf.symbols_nb; j++, elf.symbol_table++) {
        
        if (SYMBOL_TYPE(elf) == STT_FILE || SYMBOL_TYPE(elf) == STT_SECTION)
            continue;
        else if (opt->undefined_only)
        {
            symbol = resolve_undefined_symbol_type(elf.symbol_table);
            if (symbol != UNDEFINED_SYMBOL)
                add_symbol_to_lst(SYMBOL_NAME(elf), SYMBOL_ADRESS(elf), symbol, &lst, opt);
        }
        else{
            symbol = resolve_symbol_type(elf.symbol_table, SECTION_HEADER(elf));
            if (SYMBOL_IS_LOCAL(elf) && symbol != UNDEFINED_SYMBOL)
                symbol += 32;
            add_symbol_to_lst(SYMBOL_NAME(elf), SYMBOL_ADRESS(elf), symbol, &lst, opt);
        }
    }
    print_symbols_lst(lst, opt, PADDING_LEN_32);

    return TRUE;
}

static char resolve_symbol_type(Elf32_Sym   *symbol_table, Elf32_Shdr *section_hdr){

    if (ST_TYPE(symbol_table) == STT_GNU_IFUNC)
        return 'i';
    else if (ST_BIND(symbol_table) == STB_WEAK){

        if (ST_TYPE(symbol_table)== STT_OBJECT)
            return SH_INDEX(symbol_table) == SHN_UNDEF ? 'v' : 'V';
        return SH_INDEX(symbol_table) == SHN_UNDEF ? 'w' : 'W';
    }
    else if (section_hdr && (SH_TYPE(symbol_table) == SHT_PROGBITS)&& \
                SH_FLAG(section_hdr) == (SHF_ALLOC | SHF_EXECINSTR))
        return 'T';
    else if (SH_INDEX(symbol_table) == SHN_UNDEF)
        return 'U';
    else if (symbol_is_read_only_data_section(section_hdr))
        return 'R';
    else if (symbol_is_data_section(symbol_table, section_hdr))
        return SH_TYPE(symbol_table) == SHT_NOBITS ? 'B' : 'D';
    else if (ST_BIND(symbol_table) == STB_GNU_UNIQUE)
        return'u';
    else if (SH_INDEX(symbol_table) == SHN_ABS)
       return 'A';
    else if (SH_INDEX(symbol_table) == SHN_COMMON)
        return 'C';

    return '?';
}

static bool symbol_is_data_section(Elf32_Sym   *symbol_table, Elf32_Shdr *section_hdr){

    if (!section_hdr)
        return FALSE;
    
    if (ST_TYPE(symbol_table) == STT_OBJECT)
        return TRUE;
    else if (ST_TYPE(symbol_table) == STT_NOTYPE)
        return TRUE;
    else if (section_hdr->sh_type == SHT_INIT_ARRAY)
        return TRUE;
    else if (section_hdr->sh_type == SHT_FINI_ARRAY)
        return TRUE;
    else if (section_hdr->sh_type == SHT_DYNAMIC)
        return TRUE;
    else if (section_hdr->sh_flags == SHT_SHLIB)
        return TRUE;
    else if (section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS))
        return TRUE;

    return FALSE;

}

static bool symbol_is_read_only_data_section(Elf32_Shdr *section_hdr){
    
    if (!section_hdr)
        return FALSE;
    
    if (section_hdr->sh_type == SHT_REL)
        return TRUE;
    else if (section_hdr->sh_type == SHT_RELA)
        return TRUE;
    else if (section_hdr->sh_type == SHT_NOTE)
        return TRUE;
    else if ((section_hdr->sh_flags & SHF_STRINGS) > 0)
        return TRUE;
    else if (section_hdr->sh_type == SHT_PROGBITS && \
             section_hdr->sh_flags == (SHF_ALLOC) )
        return TRUE;

    return FALSE;
}

static char resolve_undefined_symbol_type(Elf32_Sym   *symbol_table){

    if (ELF32_ST_BIND(symbol_table->st_info) == STB_WEAK){
        if (ELF32_ST_TYPE(symbol_table->st_info) == STT_OBJECT)
            return symbol_table->st_shndx == SHN_UNDEF ? 'v' : '?';
        return symbol_table->st_shndx == SHN_UNDEF ? 'w' : '?';
    }
    else if (symbol_table->st_shndx == SHN_UNDEF)
        return 'U';
    
    return '?';
}

static void init_elf_struct(t_file *file, t_elf32 *elf){

    ft_memset(elf, 0, sizeof(t_elf32));

    elf->header         = (Elf32_Ehdr *)file->file;
    elf->section_hdr    = (Elf32_Shdr *)&file->file[elf->header->e_shoff];
    elf->symtab_index   = get_symtab_index(elf->header, elf->section_hdr);

    if (elf->symtab_index == -1){
        PRINT_ERROR_NO_SYMBOL_AND_EXIT(file->filename);
    }

    elf->symbol_table   = (Elf32_Sym *)(file->file + elf->section_hdr[elf->symtab_index].sh_offset);
    elf->strtab_section = &elf->section_hdr[elf->section_hdr[elf->symtab_index].sh_link];
    elf->strtab         = (char *)(file->file + elf->strtab_section->sh_offset);
    elf->symbols_nb     = elf->section_hdr[elf->symtab_index].sh_size / sizeof(Elf32_Sym);
    elf->symbol_table++;

}

static int get_symtab_index(Elf32_Ehdr *elf_header, Elf32_Shdr *section_hdr_table){

        for (int i = 0; i < elf_header->e_shnum; i++){
        if (section_hdr_table[i].sh_type == SHT_SYMTAB){
            return i;
        }
    }
    return -1;
}