#include "nm.h"
# include <sys/mman.h> // munmap()

static void open_and_map_file_and_interpret_elf(char *filename, t_file *file, t_options *options, int file_nb);


int main(int argc, char **argv){

    t_file      file;
    t_options   options;
    int         file_nb;

    file_nb = parse_options(&options, argv);

    if (file_nb == NO_FILE_ARG){
        open_and_map_file_and_interpret_elf("a.out", &file, &options, file_nb);
        return END;
    }

    for (int i = 1; i < argc; i++){
        open_and_map_file_and_interpret_elf(argv[i], &file, &options, file_nb);
    }
    return END;
}

static void open_and_map_file_and_interpret_elf(char *filename, t_file *file, t_options *options, int file_nb){

    if (!filename || !open_and_map_file(filename, file))
        return ;
    if (file_nb > 1)
        PRINT_FILENAME(filename)

    short       arch = file->file[EI_CLASS];

    switch (arch){

        case ELFCLASS32:
            interpret_symbol_table_x32(file, options);
            break;
        case ELFCLASS64:
            interpret_symbol_table_x64(file, options);
            break;
        default:
            PRINT_ERROR_WRONG_FILE_FORMAT(filename);
            break;
    }

    munmap(file->file, file->infos.st_size);
    close(file->fd);
    return ;
}
