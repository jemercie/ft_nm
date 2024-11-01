#ifndef PRINT_MACROS_H
# define PRINT_MACROS_H

# include "nm.h"

#include <unistd.h> // write
#include <stdlib.h> // exit

# define PRINT_ERROR_OPENING_FILE(filename) \
ft_putstr_fd(2, "ft_nm: an error occured opening the file ");\
ft_putstr_fd(2, filename);  write(1, "\n", 1)

# define PRINT_SYMBOL_LINE(buffer, name)\
ft_putstr_fd(1, buffer); ft_putstr_fd(1, name); write(1, "\n", 1)


# define PRINT_ERROR_GET_FILE_SIZE() \
ft_putstr_fd(2, "ft_nm: fstat : an error occured getting the file size\n");

# define PRINT_ERROR_MMAP_FAILED() \
ft_putstr_fd(2, "ft_nm: mmap : an error occured mapping the file in the memory\n");

# define PRINT_ERROR_COMPILING_REGEX_AND_EXIT \
ft_putstr_fd(2, "ft_nm: regcomp : an error occured compiling regex\n"); exit(0)

# define PRINT_ERROR_UNKNOWN_OPTION_AND_EXIT(option) \
ft_putstr_fd(2, "ft_nm: invalid option -- \'"); write(2, option, 1); ft_putstr_fd(2, "\'\n"); exit(0)

# define PRINT_ERROR_NO_SYMBOL_AND_EXIT(filename) \
ft_putstr_fd(2, "ft_nm: "); ft_putstr_fd(2, filename); ft_putstr_fd(2, ": no symbols\n"); exit(0)



#endif