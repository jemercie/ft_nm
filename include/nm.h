#ifndef INCLUDE_H
# define INCLUDE_H

# include "macros.h"
# include "print_macros.h"

#include <elf.h>         // Elf64_Ehdr, Elf32_Ehdr, EI_CLASS,
# include <sys/stat.h>   // stat, fstat()
# include <stdbool.h>    // bool type


typedef struct  s_file{

    int         fd;
    struct stat infos;
    char        *file;

}               t_file;

typedef struct  s_symbol{

    int                 index;
    char                *name;
    char                symbol;
    uint64_t            adress;
    struct s_symbol     *next;
    
}               t_symbol;


bool open_and_map_file(char *filename, t_file *file);
                    // open_and_map_file.c
bool find_and_print_symbol_table_x64(t_file *file);
                    // find_and_print_symbol_table_x64
bool find_and_print_symbol_table_x32(t_file *file);
                    // find_and_print_symbol_table_x32

#endif