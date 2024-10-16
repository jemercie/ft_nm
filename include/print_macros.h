#ifndef PRINT_MACROS_H
# define PRINT_MACROS_H

# include <stdio.h>  // printf, fprintf


# define PRINT_ERROR_OPENING_FILE(filename) \
fprintf(stderr, "ft_readelf : an error occured opening the file %s\n", filename);

# define PRINT_ERROR_GET_FILE_SIZE() \
fprintf(stderr, "ft_readelf : fstat : an error occured getting the file size\n");

# define PRINT_ERROR_MMAP_FAILED()\
fprintf(stderr, "ft_readelf : mmap : an error occured mapping the file in the memory\n");




#endif