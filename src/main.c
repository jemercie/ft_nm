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

    // if !(parse_options(argv))
    //     return END;
    printf("argc: %d\n", argc);
    for (int i = 1; i < argc; i++){
        if (argv[i] && open_and_map_file(argv[i], &file)){
            interpret_elf_header(&file);
            munmap(file.file, file.infos.st_size);
        }
    }
    return END;
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

