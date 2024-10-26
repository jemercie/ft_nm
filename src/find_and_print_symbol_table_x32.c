#include "nm.h"
#include <elf.h>

static int get_symtab_index(Elf32_Ehdr *elf_header, Elf32_Shdr *section_hdr_table);
static void     lst_add_node_sorted(t_symbol **lst, t_symbol *new);
static t_symbol *new_symbol_lst_node(char *name, int index, char symbol, uint64_t adress);
static int strcmp_no_case(char *s1, char *s2);

#include <stdlib.h>
#include <string.h>


static char resolve_symbol_type(Elf32_Sym   *symbol_table, char *strtab, Elf32_Shdr *section_hdr){
    char c = 'K';

    if (ELF32_ST_BIND(symbol_table->st_info) == STB_GNU_UNIQUE)
        c = 'u';
    else if (ELF32_ST_TYPE(symbol_table->st_info) == STT_GNU_IFUNC)
        c = 'i';
    else if (ELF32_ST_BIND(symbol_table->st_info) == STB_WEAK && ELF32_ST_TYPE(symbol_table->st_info) == STT_OBJECT){
        c = 'V';
        if (symbol_table->st_shndx == SHN_UNDEF)
            c = 'v';
    }
    else if (ELF32_ST_BIND(symbol_table->st_info) == STB_WEAK){
        c = 'W';
        if (symbol_table->st_shndx == SHN_UNDEF)
            c = 'w';
    }
    else if (symbol_table->st_shndx == SHN_ABS)
        c ='A';
    else if (symbol_table->st_shndx == SHN_COMMON)
        c = 'C';
    else if (symbol_table->st_shndx == SHN_UNDEF)
        c = 'U';
    else if (section_hdr && section_hdr->sh_type == SHT_INIT_ARRAY){
        c = 'D';
    }
    else if (symbol_table->st_shndx == SHN_UNDEF){
        if (section_hdr->sh_type == SHT_PROGBITS && section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS))
            c = 'B';
        if (section_hdr->sh_type == SHT_NOBITS && section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE | SHF_TLS))
            c = 'D';
    }
    else if (section_hdr->sh_type == SHT_PROGBITS && (section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE )))
        c = 'D';
    else if (ELF32_ST_TYPE(symbol_table->st_info) == STT_TLS )
    {
        if ((section_hdr && section_hdr->sh_type == SHT_PROGBITS) && section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE))
            c = 'D';
        else 
            c = 'B';
    }
    else if (section_hdr->sh_type == SHT_DYNAMIC || (section_hdr->sh_flags == SHT_SHLIB))
        c = 'D';
    else if ((section_hdr && section_hdr->sh_type == SHT_NOBITS && section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE)))
        c = 'B';
    else if (strcmp(strtab + section_hdr->sh_name, ".tdata") == 0 || strcmp(strtab + section_hdr->sh_name, ".tdata1") == 0)
        c = 'B';
    else if (ELF32_ST_TYPE(symbol_table->st_info) == STT_FUNC || ((section_hdr && section_hdr->sh_type == SHT_PROGBITS)&& section_hdr->sh_flags == (SHF_ALLOC | SHF_EXECINSTR)))
        c = 'T';
    else if (section_hdr && section_hdr->sh_type == SHT_PROGBITS && section_hdr->sh_flags == (SHF_ALLOC) && ELF32_ST_TYPE(symbol_table->st_info) == STT_NOTYPE)
        c = 'R';
    else if ((section_hdr && section_hdr->sh_type == SHT_NOTE)|| ((ELF32_ST_TYPE(symbol_table->st_info) == STT_OBJECT) && section_hdr->sh_flags == (SHF_ALLOC)))
         c = 'R';
    else if (ELF32_ST_TYPE(symbol_table->st_info) == STT_OBJECT || ELF32_ST_TYPE(symbol_table->st_info) == STT_NOTYPE ||\
        (section_hdr && section_hdr->sh_type == SHT_PROGBITS && section_hdr->sh_flags == (SHF_ALLOC | SHF_WRITE))){
        if ((section_hdr->sh_flags & SHF_STRINGS) > 0 || section_hdr->sh_type == SHT_REL || section_hdr->sh_type == SHT_RELA )
            c = 'R';
        else
            c = 'D';
    }
    else if ( c == 'K')
        c = '?';
    // if (section_hdr && section_hdr->sh_type == SHT_PROGBITS && section_hdr->sh_flags == (SHF_ALLOC))
    //     c = 'R';
    if (ELF32_ST_BIND(symbol_table->st_info) == STB_LOCAL && c != UNDEFINED_SYMBOL)
        c += 32;
    return c;
}
        // for v and w
        // if (strcmp(strtab+symbol_table->st_name, ".small"))
            // c = 'S';
        // else


bool find_and_print_symbol_table_x32(t_file *file){

    Elf32_Ehdr  *elf_header      = (Elf32_Ehdr *)file->file;
    Elf32_Shdr  *section_hdr     = (Elf32_Shdr *)&file->file[elf_header->e_shoff];
    int         symtab_index     = get_symtab_index(elf_header, section_hdr);
    if (symtab_index == -1){
        printf("ft_nm: filename: no symbols\n");
        return 0;
    }
    Elf32_Sym   *symbol_table    = (Elf32_Sym *)(file->file + section_hdr[symtab_index].sh_offset);

    Elf32_Shdr *strtab_section = &section_hdr[section_hdr[symtab_index].sh_link];
    char *strtab = (char *)(file->file + strtab_section->sh_offset);

    size_t symbols_nb = section_hdr[symtab_index].sh_size / sizeof(Elf32_Sym);

    t_symbol *lst = NULL;
    t_symbol *new;
    for (size_t j = 1; j < symbols_nb; j++) {
        if (ELF32_ST_TYPE(symbol_table[j].st_info) == STT_FILE || strlen(strtab+symbol_table[j].st_name) == 0)
            continue;
        else{
            char symbol = resolve_symbol_type(&symbol_table[j], strtab, symbol_table[j].st_shndx <= elf_header->e_shnum ? &section_hdr[symbol_table[j].st_shndx] : NULL);
            new = new_symbol_lst_node(strtab+symbol_table[j].st_name, j, symbol, symbol_table[j].st_value); // tester si sh_shndx est pas plus grand 
            lst_add_node_sorted(&lst, new);
            // Elf32_Shdr *section_strtab = &section_hdr[elf_header->e_shstrndx];
            // char *section_strtab_data = (char *)(file->file + section_strtab->sh_offset);
            // printf("section header name: %s, symbol table name:%s\n", (char*)section_strtab_data + section_hdr[symbol_table[j].st_shndx].sh_name, strtab+symbol_table[j].st_name);
            // printf("section header name: %s, symbol table name: %s\n", (char *)(section_strtab_data + section_hdr[symbol_table[j].st_shndx].sh_name), strtab + symbol_table[j].st_name);

        }
    }
    t_symbol *print = lst;
    for (;print; print= print->next){
            if (print->symbol == 'U' || print->symbol == 'w')
                printf("         %c %s\n", print->symbol, print->name);
            else
                printf("%08lx %c %s\n", print->adress, print->symbol, print->name);
    }
    return TRUE;
}
//symbol_type_to_letter(Elf32_ST_TYPE(symbol_table->st_info)
// name = strtab + symbol_table[j].st_name;
//if (symbol_table->st_info == STB_LOCAL && list->type != '?')
        // list->type += 32;
//     symbol_label_to_letter(            // if (Elf32_ST_TYPE(symbol_table[j].st_info) == STT_SECTION)
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


static int get_symtab_index(Elf32_Ehdr *elf_header, Elf32_Shdr *section_hdr_table){

        for (int i = 0; i < elf_header->e_shnum; i++){
        if (section_hdr_table[i].sh_type == SHT_SYMTAB){
            return i;
        }
    }
    return -1;
}

static void lst_add_node_sorted(t_symbol **lst, t_symbol *new) {
    if (!new || !new->name) return;

    if (!*lst) {
        *lst = new;
        new->next = NULL;
        return;
    }

    t_symbol *tmp = *lst;

    if (strcmp_no_case(tmp->name, new->name) > 0) {
        new->next = tmp;
        *lst = new;
        return;
    }

    while (tmp->next && strcmp_no_case(tmp->next->name, new->name) <= 0) {
        tmp = tmp->next;
    }

    new->next = tmp->next;
    tmp->next = new;
}

#include <stddef.h> 

#include <stddef.h> // pour size_t

static int strcmp_no_case(char *s1, char *s2) {
    size_t i = 0, j = 0;

    while (s1[i] || s2[j]) {
        // Ignorer les caractères non alphanumériques dans s1
        while (s1[i] && !((s1[i] >= 'A' && s1[i] <= 'Z') || 
                          (s1[i] >= 'a' && s1[i] <= 'z') || 
                          (s1[i] >= '0' && s1[i] <= '9'))) {
            i++;
        }
        // Ignorer les caractères non alphanumériques dans s2
        while (s2[j] && !((s2[j] >= 'A' && s2[j] <= 'Z') || 
                          (s2[j] >= 'a' && s2[j] <= 'z') || 
                          (s2[j] >= '0' && s2[j] <= '9'))) {
            j++;
        }

        // Si les deux chaînes sont épuisées
        if (!s1[i] && !s2[j]) {
            return 0;
        }

        // Si s1 est épuisé, mais pas s2
        if (!s1[i]) {
            return -1;
        }
        // Si s2 est épuisé, mais pas s1
        if (!s2[j]) {
            return 1;
        }

        char c1 = s1[i];
        char c2 = s2[j];

        // Gestion de la casse
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;

        // Comparaison des caractères
        if (c1 != c2) {
            return (unsigned char)c1 - (unsigned char)c2;
        }

        // Si les caractères sont identiques, on passe à la position suivante
        i++;
        j++;
    }

    // Si on sort de la boucle, ça veut dire qu'il y a des caractères restants dans une chaîne
    // On va vérifier maintenant les caractères non alphanumériques au début des chaînes.
    while (s1[i] && !((s1[i] >= 'A' && s1[i] <= 'Z') || 
                      (s1[i] >= 'a' && s1[i] <= 'z') || 
                      (s1[i] >= '0' && s1[i] <= '9'))) {
        i++;
    }
    
    while (s2[j] && !((s2[j] >= 'A' && s2[j] <= 'Z') || 
                      (s2[j] >= 'a' && s2[j] <= 'z') || 
                      (s2[j] >= '0' && s2[j] <= '9'))) {
        j++;
    }
    if (i < j)
        return 1;
    // Une fois que nous avons atteint les caractères non alphanumériques ou la fin, 
    // nous avons déjà comparé les autres caractères.

    return (unsigned char)s1[i] - (unsigned char)s2[j];
}

