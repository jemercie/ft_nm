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
    memset(options, 0, sizeof(t_options));

    for (int i = 1; argv[i]; i++){
        int ret = regex_check_arg_type(argv[i]);
        printf("return of check arg type is %d\n", ret);
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

static void set_letter_options(t_options *options, char *arg){

    for(int i = 1; arg[i]; i++){
        switch (arg[i])
        {
        case 'a':
            printf("a = true\n");
            options->debug_symbols = true;
            break;
        case 'g':
            printf("g = true\n");
            options->extern_only = true;
            break;
        case 'u':
            printf("u = true\n");
            options->undefined_only = true;
            break;
        case 'r':
            printf("r = true\n");
            options->reverse_sort = true;
            break;
        case 'p':
            printf("p = true\n");
            options->no_sort = true;
            break;
        default :
            fprintf(stderr, "unknown option %c\n", arg[i]);
            // exit(0);
            break;
        }
    }
    arg = NULL;
}

static void regex_set_full_name_options(bool *options, char *option_arg){

    static const char *regex_full_name_options_tab[] = REGEX_FULL_NAME_OPTIONS_TAB;

    for(short i = 0; i < NB_OF_OPTIONS; i++){
        if (regex_check_format(option_arg, regex_full_name_options_tab[i])){
            options[i] = true;
            printf("%d = true\n", i);
            option_arg = NULL;
            return ;
        }
    }
    fprintf(stderr, "unknown option %s\n", option_arg);
    option_arg = NULL;
    // exit(0);
}

static bool regex_check_format(const char *tested_str, const char *regex){

    regex_t reg;

    if (regcomp(&reg, regex, REG_EXTENDED | REG_NOSUB) != 0)
        return FALSE;
        // print(stderr, ERROR_COMPILING_REGEX, 1);
    int res = regexec(&reg, tested_str, (size_t) 0, NULL, 0);
    regfree(&reg);
    if (res == 0)
        return TRUE;
    return FALSE;
}
