

bool parse_options(t_options *options, char **argv){


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

static bool regex_check_format(const char *testedStr, const char *regex){

    regex_t reg;

    if (regcomp(&reg, regex, REG_EXTENDED | REG_NOSUB) != 0)
        puterr_and_exit(ERROR_COMPILING_REGEX, 1);
    int res = regexec(&reg, testedStr, (size_t) 0, NULL, 0);
    regfree(&reg);
    if (res == 0)
        return TRUE;
    return FALSE;
}

static void init_options_struct(t_options *options){

    memset(options, 0, sizeof(t_options));
}