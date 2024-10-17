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

typedef struct  s_symbol_lst{

    uint32_t        *name; // index in the string table
    unsigned char   *type;
    char *addr;
    void *next;
    
}               t_symbol_lst;


bool open_and_map_file(char *filename, t_file *file);
                    // open_and_map_file.c
bool find_and_print_symbol_table_x64(t_file *file);
                    // find_and_print_symbol_table_x64


#endif