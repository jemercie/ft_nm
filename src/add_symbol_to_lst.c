
#include "nm.h"
#include <stdlib.h> // malloc()

static t_symbol *new_symbol_lst_node(char *name, char symbol, uint64_t adress);
static void lst_add_node(t_symbol **lst, t_symbol *new);
static void lst_add_node_sorted(t_symbol **lst, t_symbol *new);
static bool is_local(char symbol);


t_symbol *add_symbol_to_lst(t_symbol *lst, char *name, char symbol, uint64_t adress, t_options *options){
                                                                    // tester si sh_shndx est pas plus grand

    if (options->extern_only && is_local(symbol))
        return lst;
        
    t_symbol *new = new_symbol_lst_node(name, symbol, adress);

    if (options->no_sort)
        lst_add_node(&lst, new);
    else 
        lst_add_node_sorted(&lst, new);
    return lst;
}

static t_symbol *new_symbol_lst_node(char *name, char symbol, uint64_t adress){

	t_symbol	*list;

	list = malloc(sizeof(t_symbol));
	if (!list)
		return (NULL);
    list->symbol = symbol;
    list->name = name;
    list->adress = adress;
	list->next = NULL;
	return (list);
}

static void lst_add_node(t_symbol **lst, t_symbol *new) {
    if (!new || !new->name) return;

    if (!*lst) {
        *lst = new;
        new->next = NULL;
        return;
    }

    t_symbol *tmp = *lst;

    while (tmp->next) {
        tmp = tmp->next;
    }

    new->next = tmp->next;
    tmp->next = new;
}

static void lst_add_node_sorted(t_symbol **lst, t_symbol *new) {
    if (!new || !new->name) return;

    if (!*lst) {
        *lst = new;
        new->next = NULL;
        return;
    }

    t_symbol *tmp = *lst;

    if (strcmp_no_case(tmp->name, new->name) > 0) {
        new->next = tmp;
        *lst = new;
        return;
    }

    while (tmp->next && strcmp_no_case(tmp->next->name, new->name) <= 0) {
        tmp = tmp->next;
    }

    new->next = tmp->next;
    tmp->next = new;
}

static bool is_lower(char c) {

    if (c >= 'a' && c <= 'z')
        return true;
    return false;
}

static bool is_local(char symbol){

    if (is_lower(symbol) && symbol != 'v' && symbol != 'w')
        return true;
    return false;
}
