#include "nm.h"
# include <sys/mman.h>

/*
        TODO

*/


static bool interpret_elf_header(t_file *file);

int main(int argc, char **argv){
    t_file file;

    for (int i = 1; i < argc; i++){
        if (open_and_map_file(argv[i], &file)){
            interpret_elf_header(&file);
        }
    }
    munmap(file.file, file.infos.st_size);
    return END;
}

static bool interpret_elf_header(t_file *file){

    short arch = file->file[EI_CLASS];
    
    switch (arch){
        case ELFCLASSNONE:
            return FALSE;
        case ELFCLASS32:
            return TRUE;
        case ELFCLASS64:
            find_and_print_symbol_table_x64(file);
            return TRUE;
    }
    return TRUE;
}

