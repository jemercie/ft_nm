
#include "nm.h"
#include <stdlib.h> // malloc()

static t_symbol *new_symbol_lst_node(char *name, char symbol, uint64_t adress);
static void lst_add_node(t_symbol **lst, t_symbol *new);
static void lst_add_node_sorted(t_symbol **lst, t_symbol *new, bool opt_r);


void add_symbol_to_lst(char *name, uint64_t adress, char symbol, t_symbol **lst, t_options *options){

    if (options->extern_only && IS_LOCAL(symbol))
        return;
        
    t_symbol *new = new_symbol_lst_node(name, symbol, adress);

    if (options->no_sort)
        lst_add_node(lst, new);
    else 
        lst_add_node_sorted(lst, new, options->reverse_sort);
}

static t_symbol *new_symbol_lst_node(char *name, char symbol, uint64_t adress){

	t_symbol	*lst;

	lst = malloc(sizeof(t_symbol));
	if (!lst)
		return (NULL);
    lst->symbol = symbol;
    lst->name = name;
    lst->adress = adress;
	lst->next = NULL;
	return (lst);
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

static void lst_add_node_sorted(t_symbol **lst, t_symbol *new, bool opt_r) {
    if (!new || !new->name) return;

    if (!*lst) {
        *lst = new;
        new->next = NULL;
        return;
    }

    t_symbol *tmp = *lst;

    if (strcmp_no_case(tmp->name, new->name, opt_r) >= 0) {
        new->next = tmp;
        *lst = new;
        return;
    }

    while (tmp->next && strcmp_no_case(tmp->next->name, new->name, opt_r) <= 0) {
        tmp = tmp->next;
    }

    new->next = tmp->next;
    tmp->next = new;
}

