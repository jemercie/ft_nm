# include "nm.h"
# include "regex_macros.h"
# include <regex.h>
# include <string.h>

static int  regex_check_arg_type(char *arg);
static void regex_set_full_name_options(bool *options, char *option);
static void set_letter_options(t_options *options, char *arg);
static bool regex_check_format(const char *tested_str, const char *regex);


int parse_options(t_options *options, char **argv){

    int nb_args = 0;
    ft_memset(options, 0, sizeof(t_options));
    for (int i = 1; argv[i]; i++){
        int ret = regex_check_arg_type(argv[i]);
        switch (ret)
        {
        case FULL_NAME_OPTION:
            regex_set_full_name_options((bool *)options, argv[i]);
            argv[i] = NULL;
            break;
        case LETTER_OPTION:
            set_letter_options(options, argv[i]);
            argv[i] = NULL;
            break;
        case FILENAME:
            nb_args++;
            break;
        }
    }
    if (options->help){
        PRINT_USAGE_AND_EXIT(0);
    }
    return nb_args;
}

static int regex_check_arg_type(char *arg){

    static const char *regex_arg_types_tab[] = REGEX_CHECK_ARG_TYPE;

    for (short i = 0; i < NB_OF_ARG_TYPE; i++){
        if (regex_check_format(arg, regex_arg_types_tab[i]))
            return i;
    }
    return FILENAME;
}

static void set_letter_options(t_options *options, char *option_arg){

    for(int i = 1; option_arg[i]; i++){
        switch (option_arg[i])
        {
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
        case '?':
            options->help = true;
            break;
        default :
            PRINT_ERROR_UNKNOWN_OPTION(&option_arg[i]);
            PRINT_USAGE_AND_EXIT(1);
        }
    }
    option_arg = NULL;
}

static void regex_set_full_name_options(bool *options, char *option_arg){

    static const char *regex_full_name_options_tab[] = REGEX_FULL_NAME_OPTIONS_TAB;
    short i = 0;

    for( ; i < NB_OF_OPTIONS; i++){
        if (regex_check_format(option_arg, regex_full_name_options_tab[i])){
            options[i] = true;
            option_arg = NULL;
            return ;
        }
    }
    PRINT_ERROR_UNKNOWN_OPTION(&option_arg[i]);
    PRINT_USAGE_AND_EXIT(1);
}

static bool regex_check_format(const char *tested_str, const char *regex){

    regex_t reg;

    if (regcomp(&reg, regex, REG_EXTENDED | REG_NOSUB) != 0){
        PRINT_ERROR_COMPILING_REGEX_AND_EXIT;
        exit(0);
    }
    int res = regexec(&reg, tested_str, (size_t) 0, NULL, 0);
    regfree(&reg);
    if (res == 0)
        return TRUE;
    return FALSE;
}
