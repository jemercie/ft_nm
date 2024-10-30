#ifndef REGEX_H
# define REGEX_H

# define OPTION_FORMAT

-a --debug-syms 
-g --extern_only
-u --undefined_only

-r --reverse-sort
-p --no-sort   p mm si r on

# define REGEX_FULL_NAME_OPTIONS_TAB{\
    "";

};

# define NB_OF_ARG_TYPE 3

# define REGEX_CHECK_ARG_TYPE    {\
    "^-[ -~]*$",\
    "^--[ -~]*$",
};



enum{
    LETTER_OPTION,
    FULL_NAME_OPTION,
    FILENAME,
};

#endif