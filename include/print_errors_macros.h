#ifndef PRINT_MACROS_H
# define PRINT_MACROS_H

#include <unistd.h> // write
#include <stdlib.h> // exit

# define PRINT_FILENAME(filename){ \
write(1, "\n", 1); ft_putstr_fd(1, filename); write(1, ":\n",2);}

# define PRINT_SYMBOL_LINE(buffer, name)\
ft_putstr_fd(1, buffer); ft_putstr_fd(1, name); write(1, "\n", 1)

# define PRINT_ERROR_OPENING_FILE(filename) \
ft_putstr_fd(2, "ft_nm: '"); ft_putstr_fd(2, filename); \
ft_putstr_fd(2, "': No such file\n");

# define PRINT_ERROR_COMPILING_REGEX "An error occured compiling regex.\n"

# define PRINT_ERROR_IS_A_DIRECTORY(filename) \
ft_putstr_fd(2, "ft_nm: Warning: '"); ft_putstr_fd(2, filename); ft_putstr_fd(2, "' is a directory\n")

# define PRINT_ERROR_GET_FILE_SIZE() \
ft_putstr_fd(2, "ft_nm: fstat : an error occured getting the file size\n");

# define PRINT_ERROR_MMAP_FAILED() \
ft_putstr_fd(2, "ft_nm: mmap : an error occured mapping the file in the memory\n");

# define PRINT_ERROR_COMPILING_REGEX_AND_EXIT \
ft_putstr_fd(2, "ft_nm: regcomp : an error occured compiling regex\n"); exit(1)

# define PRINT_ERROR_UNKNOWN_OPTION(option) \
ft_putstr_fd(2, "ft_nm: invalid option -- \'"); write(2, option, 1); ft_putstr_fd(2, "\'\n")

# define PRINT_ERROR_NO_SYMBOL_AND_EXIT(filename) \
ft_putstr_fd(2, "ft_nm: "); ft_putstr_fd(2, filename); ft_putstr_fd(2, ": no symbols\n"), exit(1)

# define PRINT_ERROR_WRONG_FILE_FORMAT(filename)\
ft_putstr_fd(2, "ft_nm: "); ft_putstr_fd(2, filename); ft_putstr_fd(2, ": file format not recognized\n")

# define PRINT_USAGE_AND_EXIT(exit_code) \
ft_putstr_fd(2, "Usage: nm [option(s)] [file(s)]\n\
 List symbols in [file(s)] (a.out by default).\n\
 The options are:\n\
  -g, --extern-only      Display only external symbols\n\
  -p, --no-sort          Do not sort the symbols\n\
  -r, --reverse-sort     Reverse the sense of the sort\n\
  -u, --undefined-only   Display only undefined symbols\n\
  -?, --help             Display this information\n\
nm: supported targets: x86_32, x64, object files, .so\n"); exit(exit_code)

#endif