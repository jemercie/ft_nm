#ifndef REGEX_MACROS_H
# define REGEX_MACROS_H

# define NB_OF_ARG_TYPE 3
# define NB_OF_OPTIONS 5

# define REGEX_FULL_NAME_OPTIONS_TAB {\
    "^--debug-syms$",\
    "^--extern-only$",\
    "^--undefined-only$",\
    "^--reverse-sort$",\
    "^--no-sort$",\
};


# define REGEX_CHECK_ARG_TYPE {\
    "^--[ -~]*$",\
    "^-[ -~]*$",\
};

enum{
    FULL_NAME_OPTION,
    LETTER_OPTION,
    FILENAME,
};      // ARG TYPES

#endif