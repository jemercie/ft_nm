#ifndef INCLUDE_H
# define INCLUDE_H

# include "macros.h"
# include "print_errors_macros.h"

# include <elf.h>        // Elf64_Ehdr, Elf32_Ehdr, EI_CLASS,
# include <sys/stat.h>   // stat, fstat()
# include <stdbool.h>    // bool type

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




bool        open_and_map_file(char *filename, t_file *file);
                    // open_and_map_file.c

int         parse_options(t_options *options, char **argv);
                    // regex_parse_options.c

bool        interpret_symbol_table_x64(t_file *file, t_options *options);
                    // find_and_print_symbol_table_x64

bool        interpret_symbol_table_x32(t_file *file, t_options *options);
                    // find_and_print_symbol_table_x32

void        add_symbol_to_lst(char *name, uint64_t adress, char symbol, t_symbol **lst, t_options *options);
                    // add_symbol_to_lst.c

void        print_symbols_lst(t_symbol *lst, t_options *options, int padding_len);
                    // print_symbols_lst.c

int         strcmp_no_case(char *s1, char *s2, bool opt_r);
void        ft_putstr_fd( int fd, char *str);
void        *ft_memset(void *s, int c, size_t n);
size_t      ft_strlen(const char *str);
                    // utils.c



#endif