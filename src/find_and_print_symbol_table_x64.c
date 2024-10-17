#include "nm.h"
#include <elf.h>

static Elf64_Sym *find_symbol_table(Elf64_Ehdr *elf_header, Elf64_Shdr *section_hdr_table);
static bool get_symbol_table(Elf64_Shdr *section_hdr, Elf64_Sym *symbol_table, Elf64_Ehdr *elf_header);


bool find_and_print_symbol_table_x64(t_file *file){

    Elf64_Ehdr *elf_header      = (Elf64_Ehdr *)&file->file[0];
    Elf64_Shdr *section_hdr     = (Elf64_Shdr *)&file->file[elf_header->e_shoff];
    Elf64_Sym  *symbol_table    = find_symbol_table(elf_header, section_hdr);

    if (symbol_table == NULL)
        return FALSE;
    get_symbol_table(section_hdr, symbol_table, elf_header);
    return TRUE;
}

static Elf64_Sym *find_symbol_table(Elf64_Ehdr *elf_header, Elf64_Shdr *section_hdr_table){

    for (int i = 0; i < elf_header->e_shnum; i++){
        if (section_hdr_table[(i * elf_header->e_shentsize)].sh_type == SHT_SYMTAB)
            return ((Elf64_Sym *)&(section_hdr_table[(i * elf_header->e_shentsize)].sh_type));
    }
    return NULL;
}

static bool get_symbol_table(Elf64_Shdr *section_hdr, Elf64_Sym *symbol_table, Elf64_Ehdr *elf_header){

    // t_symbol_lst *symbol_lst = NULL;

    for (int i = 0; (i*section_hdr->sh_entsize) < section_hdr->sh_size; i++){
            printf("0x16%hu %hu %u\n", symbol_table->st_shndx, symbol_table->st_info, elf_header->e_shstrndx[&symbol_table->st_name]);

            // get_symbol_line(symbol_lst, &symbol_table[i*section_hdr->sh_size]);
    }
    return TRUE;
}

// static bool get_symbol_line(t_symbol_lst * symbol_lst, Elf64_Sym *symbol_table){
//     printf("0x16%p %c %s\n", symbol_table->st_shndx, symbol_table->st_info, )
// }