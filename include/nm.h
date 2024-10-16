#ifndef INCLUDE_H
# define INCLUDE_H

# include "macros.h"
# include "print_macros.h"

#include <elf.h>         // Elf64_Ehdr, Elf32_Ehdr, EI_CLASS,
# include <sys/stat.h>   // stat, fstat()
# include <stdbool.h>    // bool type


typedef struct s_file{

    int         fd;
    struct stat infos;
    char        *file;

}              t_file;


bool open_and_map_file(char *filename, t_file *file);
                    // open_and_map_file.c



#endif