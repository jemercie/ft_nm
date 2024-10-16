#include <unistd.h>
# define ratatouille "ratatouille"

int global;

void print_caca(void){
    char caca[5] = {'c','a','c','a','\0'};
    write(1, &caca, 4);
}
void print_ratatouille(void){
    write(1, &ratatouille, 11);
}
void print_extern(void){
    global = 5;
    write(1, &global, 1);
}

int main(void){
    print_caca();
    print_ratatouille();
    print_extern();
    return 0;
}