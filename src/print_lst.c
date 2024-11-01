# include "nm.h"
#include <stdlib.h> // malloc()

static char	*ft_int_to_hex_str(int num);
static char *fill_padding_with_zeros_adress(short padding_len, uint64_t adress);
static char *fill_padding_with_spaces(short padding_len);
static char *add_symbol_to_buffer(char *buffer, char symbol, short padding_len);

void print_lst(t_symbol *lst, short padding_len){

    t_symbol *symbol = lst;
	static char *buffer;

    for (;symbol; symbol= symbol->next){
        if (symbol->symbol == 'U' || symbol->symbol == 'w')
			buffer = fill_padding_with_spaces(padding_len);
        else
			buffer = fill_padding_with_zeros_adress(padding_len, symbol->adress);
		buffer = add_symbol_to_buffer(buffer, symbol->symbol, padding_len-1);

        PRINT_SYMBOL_LINE(buffer, symbol->name);
    }
}

void recursive_print_lst(t_symbol *lst, short padding_len){

    t_symbol *symbol = lst;
	static char *buffer;

	if (symbol->next)
		recursive_print_lst(symbol->next, padding_len);

    if (symbol->symbol == 'U' || symbol->symbol == 'w')
		buffer = fill_padding_with_spaces(padding_len);
    else
		buffer = fill_padding_with_zeros_adress(padding_len, symbol->adress);
	buffer = add_symbol_to_buffer(buffer, symbol->symbol, padding_len-1);

    PRINT_SYMBOL_LINE(buffer, symbol->name);
}

static char *fill_padding_with_zeros_adress(short padding_len, uint64_t adress){

    short 		i 			= 0;
    char 		*adress_str = ft_int_to_hex_str(adress);
    int 		adress_len	= ft_strlen(adress_str);
	static char buffer[PADDING_LEN_64 + 1];

	ft_memset(&buffer, 0, sizeof(char) * (PADDING_LEN_64 + 1));
    // if (adress_len < padding_len) -> error

    for (; i < (padding_len - adress_len - SYMBOL_PADDING); i++){
        buffer[i] = '0';
    }
    for (int j = 0; i < (padding_len - SYMBOL_PADDING); i++, j++){
        buffer[i] = adress_str[j];
    }
	for (int j = 0; j < SYMBOL_PADDING; j++, i++)
		buffer[i+j]=' ';
	
	for (; i <= PADDING_LEN_64; i++)
		buffer[i] = '\0';
	
	free(adress_str);
	return (char *)&buffer;
}

static char *fill_padding_with_spaces(short padding_len){
	
	static char buffer[17];

	ft_memset(&buffer, 0, sizeof(char) * (PADDING_LEN_64+1));
	ft_memset(&buffer, ' ', padding_len);
	return (char *)&buffer;
}

static char *add_symbol_to_buffer(char *buffer, char symbol, short padding_len){

	buffer[padding_len--] = ' ';
	buffer[padding_len--] = symbol;
	buffer[padding_len--] = ' ';
	return buffer;
}

static char *ft_int_to_hex_str(int num) {
    int i;
    char *str;
    const char hex_digits[] = HEX_DIGITS;
    
    if (num == 0) {
        str = malloc(sizeof(char) * 2);
        if (!str)
			return NULL;
        str[0] = '0'; str[1] = '\0';
        return str;
    }

    char tmp[10]; 

    for (i = 0; num > 0; i++) {
        int remainder_index = num % 16;
        tmp[i] = hex_digits[remainder_index];
        num /= 16;
    }

    str = malloc(sizeof(char) * (i + 1));
    if (!str) 
		return NULL;

    for (int j = 0; j < i; j++) {
        str[j] = tmp[i - j - 1];
    }
    str[i] = '\0';

    return str;
}