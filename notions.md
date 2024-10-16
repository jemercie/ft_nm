
### nm

_name mangling_

**use:**
    nm \[*options*\] \[*objfile...\]

###### 3 colonnes:

    | virtual adress | symbol type | symbol name |
    | 000000000040037c| r | __abi_tag |
    | 0000000000407208| B | __bss_start |
    | 00000000004031d0| T | check_count_option |
    | 0000000000401730| t | check_flood_and_interval |


###### types of symbol:
    A           Absolute symbol, will not be changed by further linking
    B - b       In the uninitialized data section (known as BSS)
    C           Common symbols, uninitialized data. When linking, 
                multiple common symbols may appear with the same name. If 
                the symbol is defined anywhere, the common symbols 
                are treated as undefined references.
    D -  d      In the initialized data section
    G - g       Initialized data section for small objects. Some object 
                file formats permit more efficient access to small data 
                objects, such as a global int variable as opposed to a 
                large global array.
    i           For PE format files this indicates that the symbol is  
                in a section specific to the implementation of DLLs
    N           Debugging symbol
    p           The symbols is in a stack unwind section.
    R - r       The symbol is in a read only data section.
    S - s       The symbol is in an uninitialized data section for small objects
    T - t       The symbol is in the text (code) section.
    U           Undefined symbol right now
    V - v       The symbol is a weak object. When a weak defined symbol is linked 
                with a normal defined symbol, the normal defined symbol is used with
                no error. When a weak undefined symbol is linked and the symbol is 
                not defined, the value of the weak symbol becomes zero with no 
                error. On some systems, uppercase indicates that a default value has
                been specified.
    W - w       The symbol is a weak symbol that has not been specifically tagged as 
                a weak object symbol. When a weak defined symbol is linked with a 
                normal defined symbol, the normal defined symbol is used with no 
                error. When a weak undefined symbol is linked and the symbol is not 
                defined, the value of the symbol is determined in a system-specific 
                manner without error. On some systems, uppercase indicates that a 
                default value has been specified.
    - X         Affiche la valeur et la taille d'un symbole sous la forme d'un nombre 
                hexadécimal plutôt que d'un nombre décimal.
    -           The symbol is a stabs symbol in an a.out object file. In this case,
                the next values printed are the stabs other field, the stabs desc 
                field, and the stab type. Stabs symbols are used to hold debugging 
                information.
    ?           The symbol type is unknown, or object file format specific.


Lower case is **Local** symbol
Uppercase is **External** symbol


options cool:
    -A
    -n (display in sorted order)
    -g only external symbols (uppercase)
    -S display size

