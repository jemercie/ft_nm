#include "nm.h"
#include <elf.h>

static int get_symtab_index(Elf64_Ehdr *elf_header, Elf64_Shdr *section_hdr_table);
static void     lst_add_node_sorted(t_symbol **lst, t_symbol *new);
static t_symbol *new_symbol_lst_node(char *name, int index, char symbol, uint64_t adress);
static int strcmp_no_case(const char *s1, const char *s2);

#include <stdlib.h>
#include <string.h>


static char resolve_symbol_type(Elf64_Sym   *symbol_table, Elf64_Shdr *section_hdr){
    char c = 'K';

    if (symbol_table->st_shndx == SHN_ABS)
        c ='A';
    else if (symbol_table->st_shndx == SHN_COMMON)
        c = 'C';
    else if (symbol_table->st_shndx == SHN_UNDEF){
        if (ELF64_ST_BIND(symbol_table->st_info) == STB_WEAK)
            c = 'w';
        else
            c = 'U';
    }
    else if (section_hdr && section_hdr->sh_type == SHT_NOBITS && section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE))
        c = 'B';
    else if (ELF64_ST_TYPE(symbol_table->st_info) == STT_OBJECT ||\
            (section_hdr && section_hdr->sh_type == SHT_PROGBITS && section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE))){
                if (ELF64_ST_BIND(symbol_table->st_info) == STB_WEAK)
                    c = 'W';
                else if (section_hdr->sh_flags == (SHF_ALLOC))
                    c = 'R';
                else
                    c = 'D';
            // printf("%lu, flags: %d, is true: %d, alloc: %d\n", section_hdr->sh_flags, (SHF_ALLOC | SHF_WRITE), (section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE)), (SHF_ALLOC));
            }
    else if (ELF64_ST_TYPE(symbol_table->st_info) == STT_FUNC)
        c = 'T';
    else if ( c == 'K')
        c = '?';

    if (section_hdr && section_hdr->sh_type == SHT_PROGBITS && section_hdr->sh_flags == (SHF_ALLOC))
        c = 'R';

    if (ELF64_ST_BIND(symbol_table->st_info) == STB_LOCAL && c != UNDEFINED_SYMBOL)
        c += 32;

    return c;
}
        // for v and w
        // if (strcmp(strtab+symbol_table->st_name, ".small"))
            // c = 'S';
        // else


bool find_and_print_symbol_table_x64(t_file *file){

    Elf64_Ehdr  *elf_header      = (Elf64_Ehdr *)file->file;
    Elf64_Shdr  *section_hdr     = (Elf64_Shdr *)&file->file[elf_header->e_shoff];
    int         symtab_index     = get_symtab_index(elf_header, section_hdr);
    if (symtab_index == -1){
        printf("ft_nm: filename: no symbols\n");
        return 0;
    }
    Elf64_Sym   *symbol_table    = (Elf64_Sym *)(file->file + section_hdr[symtab_index].sh_offset);

    Elf64_Shdr *strtab_section = &section_hdr[section_hdr[symtab_index].sh_link];
    char *strtab = (char *)(file->file + strtab_section->sh_offset);

    size_t symbols_nb = section_hdr[symtab_index].sh_size / sizeof(Elf64_Sym);

    t_symbol *lst = NULL;
    t_symbol *new;
    for (size_t j = 1; j < symbols_nb; j++) {
        if (ELF64_ST_TYPE(symbol_table[j].st_info) == STT_FILE)
            continue;
        else{
            char symbol = resolve_symbol_type(&symbol_table[j], symbol_table[j].st_shndx <= elf_header->e_shnum ? &section_hdr[symbol_table[j].st_shndx] : NULL);
            new = new_symbol_lst_node(strtab+symbol_table[j].st_name, j, symbol, symbol_table[j].st_value); // tester si sh_shndx est pas plus grand 
            lst_add_node_sorted(&lst, new);

        }
    }
    t_symbol *print = lst;
    for (;print; print= print->next){
            if (print->symbol == 'U' || print->symbol == 'w')
                printf("                 %c %s\n", print->symbol, print->name);
            else
                printf("%016lx %c %s\n", print->adress, print->symbol, print->name);
    }
    return TRUE;
}
//symbol_type_to_letter(ELF64_ST_TYPE(symbol_table->st_info)
// name = strtab + symbol_table[j].st_name;
//if (symbol_table->st_info == STB_LOCAL && list->type != '?')
        // list->type += 32;
//     symbol_label_to_letter(            // if (ELF64_ST_TYPE(symbol_table[j].st_info) == STT_SECTION)
            //     printf("%s\n", strtab + section_hdr[symbol_table[j].st_shndx].sh_name);&list->type, symbol_table->st_shndx);



static t_symbol *new_symbol_lst_node(char *name, int index, char symbol, uint64_t adress){

	t_symbol	*list;

	list = malloc(sizeof(t_symbol));
	if (!list)
		return (NULL);
    list->symbol = symbol;
	list->index = index;
    list->name = name;
    list->adress = adress;
	list->next = NULL;
	return (list);
}


static int get_symtab_index(Elf64_Ehdr *elf_header, Elf64_Shdr *section_hdr_table){

        for (int i = 0; i < elf_header->e_shnum; i++){
        if (section_hdr_table[i].sh_type == SHT_SYMTAB){
            return i;
        }
    }
    return -1;
}

static void lst_add_node_sorted(t_symbol **lst, t_symbol *new) {

    if (!new) return;

    if (!*lst) {
        *lst = new;
        return;
    }

    t_symbol *tmp = *lst;

    if (strcmp_no_case(tmp->name, new->name) > 0) {
        new->next = tmp;
        *lst = new;
        return;
    }

    while (tmp->next && strcmp_no_case(tmp->next->name, new->name) < 0) {
        tmp = tmp->next;
    }

    new->next = tmp->next;
    tmp->next = new;
}

static int strcmp_no_case(const char *s1, const char *s2) {
    size_t i = 0, j = 0;

    while (s1[i] || s2[j]) {
        while (s1[i] && !(s1[i] >= 'A' && s1[i] <= 'Z') && !(s1[i] >= 'a' && s1[i] <= 'z')) {
            i++;
        }
        while (s2[j] && !(s2[j] >= 'A' && s2[j] <= 'Z') && !(s2[j] >= 'a' && s2[j] <= 'z')) {
            j++;
        }

        if (!s1[i] && !s2[j]) {
            return 0;
        }

        if (!s1[i] || !s2[j]) {
            return (unsigned char)s1[i] - (unsigned char)s2[j];
        }

        char c1 = s1[i];
        char c2 = s2[j];
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;

        if (c1 != c2) {
            return (unsigned char)c1 - (unsigned char)c2;
        }

        i++;
        j++;
    }

    return 0; // Both strings are equal
}
