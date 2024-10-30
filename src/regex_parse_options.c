# include "nm.h"
#include <regex.h>

int parse_options(t_options *options, char **argv){

    int nb_args = 0;

    init_options_struct(options);
    for (int i = 0; argv[i]; i++){
        switch (check_arg_type(argv[i]))
        {
        case LETTER_OPTION:
            set_letter_options(options, argv[i]);
            break;
        case FULL_NAME_OPTION:
            /* code */
            break;
        case FILENAME:
            nb_args++;
            break;
        
        }
    }
    return nb_args;
}

static void init_options_struct(t_options *options){

    memset(options, 0, sizeof(t_options));
}


static int check_arg_type(char *arg){
    static const char *regex_arg_types_tab[] = REGEX_CHECK_ARG_TYPE;

    for (short i = 0; i < NB_OF_ARG_TYPE; i++){
        if (regex_check_format(arg, regex_arg_types_tab[i]))
            return i;
    }
    return FILENAME;
}

static void set_full_name_options(t_options *options, char*arg){
    static const char *regex_full_name_options_tab[] = ;
}


static void set_letter_options(t_options *options, char *arg){

    for(int i = 1; arg[i]; i++){
        switch (arg[i])
        {
        case 'a':
            options->debug_symbols = true;
            break;
        case 'g':
            options->extern_only = true;
            break;
        case 'u':
            options->undefined_only = true;
            break;
        case 'r':
            options->reverse_sort = true;
            break;
        case 'p':
            options->no_sort = true;
            break;
        default :
            // puterr_and_exit;
            break;
        }
    }
    *arg = NULL;
}

static bool regex_get_and_set_boolean_value(bool *opt, char *option){

    const char *regex_tab_bool_option[] = REGEX_LIST_BOOL_OPTION;

    for( int i = 0; i < NB_OF_BOOLEAN_OPTIONS; i++){
        if (regex_check_format(option, regex_tab_bool_option[i])){
            opt[i] = true;
            return TRUE;
        }
    }
    return FALSE;
}

static bool regex_check_format(const char *tested_str, const char *regex){

    regex_t reg;

    if (regcomp(&reg, regex, REG_EXTENDED | REG_NOSUB) != 0)
        print(stderr, ERROR_COMPILING_REGEX, 1);
    int res = regexec(&reg, tested_str, (size_t) 0, NULL, 0);
    regfree(&reg);
    if (res == 0)
        return TRUE;
    return FALSE;
}
