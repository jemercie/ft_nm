#include "macros.h"
#include "nm.h"
#include "print_macros.h"

#include <sys/types.h>  // open
#include <fcntl.h>      // open
#include <string.h>     // memset
#include <sys/mman.h>   // mmap


static bool open_file(char *file, int *fd);
static bool get_file_size(t_file *file);
static bool map_file(t_file *file);

// nm test_script.sh 
// nm: test_script.sh: file format not recognized


bool open_and_map_file(char *filename, t_file *file){

    memset(file, 0, sizeof(t_file));
    if (!open_file(filename, &file->fd))
        return FALSE;
    if (!get_file_size(file))
        return FALSE;
    if (!map_file(file))
        return FALSE;
    return TRUE;
}

static bool open_file(char *filename, int *fd){

    *fd = open(filename, O_RDONLY);
    if (*fd < 0){
        PRINT_ERROR_OPENING_FILE(filename);
        return FALSE;
    }
    return TRUE;
}

static bool get_file_size(t_file *file){

    if (fstat(file->fd, &file->infos) < 0){
        PRINT_ERROR_GET_FILE_SIZE();
        return FALSE;
    }
    return TRUE;
}

static bool map_file(t_file *file){

    file->file = mmap(NULL, file->infos.st_size, PROT_READ, MAP_PRIVATE, file->fd, 0);
    if (file->file == (void *)-1){
        PRINT_ERROR_MMAP_FAILED();
        return FALSE;
    }
    return TRUE;
}
