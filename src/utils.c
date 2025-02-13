#include "nm.h"
#include <unistd.h> // write()

static bool ft_isalnum(int c);
static bool is_upper(char c);
static int  strcmp_case(char *s1, char *s2, bool opt_r);

int strcmp_no_case(char *s1, char *s2, bool opt_r) {
    
    size_t i = 0, j = 0, _s1 = 0, _s2 = 0;

    for (; s1[i] || s2[j]; i++, j++) {

        for (; (s1[i] && !ft_isalnum(s1[i])); i++, _s1++);
        for (; (s2[j] && !ft_isalnum(s2[j])); j++, _s2++);

        if (!s1[i] && !s2[j])
            return ((opt_r) ? 1 : 0);
        if (!s1[i]) 
            return -1;
        if (!s2[j]) 
            return 1;

        char c1 = s1[i];
        char c2 = s2[j];

        is_upper(c1) ? c1 += 32 : c1;
        is_upper(c2) ? c2 += 32 : c2;
        if (c1 != c2)
            return (unsigned char)c1 - (unsigned char)c2;
    }
    return (strcmp_case(s1, s2, opt_r));

}

static int strcmp_case(char *s1, char *s2, bool opt_r){

    int i = 0, j = 0;

    for (; s1[i] && s2[j]; i++, j++){
    
        for (; (s1[i] && !ft_isalnum(s1[i])); i++);
        for (; (s2[j] && !ft_isalnum(s2[j])); j++);
        if (i < j || s1[i] < s2[j])
            return 1;
        else if (i > j || s1[i] > s2[j])
            return 0;

    }
    return ((opt_r) ? 1 : 0);
}

static bool ft_isalnum(int c){

    if ((c > 47 && c < 58) || (c > 64 && c < 91) || (c > 96 && c < 123))
        return TRUE;
    else
        return FALSE;
}

static bool is_upper(char c) {
    if (c >= 'A' && c <= 'Z')
        return true;
    return false;
}

void    ft_putstr_fd( int fd, char *str){

    if (fd < 0 || !str)
        return ;
    write(fd, str, ft_strlen(str));
}

void    *ft_memset(void *s, int c, size_t n){

        char    *str = s;

        str = s;
        for (size_t i = 0; i < n; i++){
            str[i] = c;
        }
        return (s);
}

size_t  ft_strlen(const char *str){

    if (!str)
        return (0);

    size_t     i = 0;
    while (str[i])
        i++;

    return (i);
}

