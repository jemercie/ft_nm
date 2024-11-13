#ifndef INCLUDE_H
# define INCLUDE_H

# include "macros.h"
# include "errors.h"
# include "print_macros.h"

#include <elf.h>         // Elf64_Ehdr, Elf32_Ehdr, EI_CLASS,
# include <sys/stat.h>   // stat, fstat()
# include <stdbool.h>    // bool type

# define NO_FILE_ARG    0

typedef struct  s_file{

    int         fd;
    struct stat infos;
    char        *file;
    char        *filename;
}               t_file;


typedef struct  s_symbol{

    int                 index;
    char                *name;
    char                symbol;
    uint64_t            adress;
    struct s_symbol     *next;
}               t_symbol;


typedef struct  s_options{

    bool    extern_only;
    bool    undefined_only;
    bool    reverse_sort;
    bool    no_sort;
    bool    help;
}               t_options;

typedef struct  s_elf32{

    Elf32_Ehdr  *header;
    Elf32_Shdr  *section_hdr;
    int         symtab_index ;
    Elf32_Sym   *symbol_table;
    Elf32_Shdr  *strtab_section;
    char *strtab;
    size_t symbols_nb;

}               t_elf32;

typedef struct  s_elf64{

    Elf64_Ehdr  *header;
    Elf64_Shdr  *section_hdr;
    int         symtab_index ;
    Elf64_Sym   *symbol_table;
    Elf64_Shdr  *strtab_section;
    char *strtab;
    size_t symbols_nb;

}               t_elf64;

bool        open_and_map_file(char *filename, t_file *file);
                    // open_and_map_file.c

int         parse_options(t_options *options, char **argv);
                    // regex_parse_options.c

bool        interpret_symbol_table_x64(t_file *file, t_options *options);
                    // find_and_print_symbol_table_x64

bool        interpret_symbol_table_x32(t_file *file, t_options *options);
                    // find_and_print_symbol_table_x32

t_symbol    *add_symbol_to_lst(char *name, uint64_t adress, char symbol, t_symbol *lst, t_options *options);
                    // add_symbol_to_lst.c

void        print_symbols_lst(t_symbol *lst, t_options *options, int padding_len);
                    // print_symbols_lst.c

int         strcmp_no_case(char *s1, char *s2, bool opt_r);
void        ft_putstr_fd( int fd, char *str);
void        *ft_memset(void *s, int c, size_t n);
size_t      ft_strlen(const char *str);
                    // utils.c



#endif