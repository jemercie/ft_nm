#include "nm.h"
# include <sys/mman.h>

/*
        TODO
    - compile libft properly
    - replace every forbidden functions (printf etc)
    - make the resolve symbol type function less ugly
    - faire un truc pour les symboles de debug
    - manage options: 
            - a (dont skip stt_files f flag up)
            - g extern only (uppercase)
            - u (undefined only)
            - r reverse sort
            - p no sort

*/


static bool interpret_elf_header(t_file *file);

int main(int argc, char **argv){
    t_file file;
    t_options options;

    // if (parse_options(& options, argv) == NO_ARGS){
        // open_and_map_file_and_interpret_elf_header("a.out", &file)
        // return END;
    // }

    for (int i = 1; i < argc; i++){
            open_and_map_file_and_interpret_elf_header(argv[i], &file);
    }
    return END;
}

static void open_and_map_file_and_interpret_elf_header(char *filename, t_file *file){

    if (!filename|| !open_and_map_file(filename, file))
        return ;
    interpret_elf_header(&file);
    munmap(file->file, file->infos.st_size);

    return ;
}

static bool interpret_elf_header(t_file *file){

    short arch = file->file[EI_CLASS];
    
    switch (arch){
        case ELFCLASSNONE:
            return FALSE;
        case ELFCLASS32:
            find_and_print_symbol_table_x32(file);
            return TRUE;
        case ELFCLASS64:
            find_and_print_symbol_table_x64(file);
            return TRUE;
    }
    return TRUE;
}

