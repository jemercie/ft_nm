#include "nm.h"
# include <sys/mman.h>

/*
        TODO
    - make the resolve symbol type function less ugly
    - rm errors.h 
    - put some complexity inside macros
    - print and free lst
*/


static void open_and_map_file_and_interpret_elf(char *filename, t_file *file, t_options *options);


int main(int argc, char **argv){

    t_file file;
    t_options options;

    if (parse_options(&options, argv) == NO_FILE_ARG){
        open_and_map_file_and_interpret_elf("a.out", &file, &options);
        return END;
    }

    for (int i = 1; i < argc; i++){
        open_and_map_file_and_interpret_elf(argv[i], &file, &options);
    }
    return END;
}

static void open_and_map_file_and_interpret_elf(char *filename, t_file *file, t_options *options){

    if (!filename || !open_and_map_file(filename, file))
        return ;
    
    short       arch = file->file[EI_CLASS];


    switch (arch){

        case ELFCLASS32:
            interpret_symbol_table_x32(file, options);
            break;
        case ELFCLASS64:
            interpret_symbol_table_x64(file, options);
            break;
        default:
            PUT_ERROR_WRONG_FILE_FORMAT(filename);
            break;
    }

    munmap(file->file, file->infos.st_size);
    close(file->fd);
    return ;
}
