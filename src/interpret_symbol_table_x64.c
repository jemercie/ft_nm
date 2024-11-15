
#define Elfw(x) Elf64_##x
#define ELFF(func_name) func_name##_x64
#define PADDING_LEN                          19
#define ST_TYPE(symbol_table)  (ELF64_ST_TYPE(symbol_table->st_info))
#define ST_BIND(symbol_table)  (ELF64_ST_BIND(symbol_table->st_info))
#define SYMBOL_TYPE(elf)       (ELF64_ST_TYPE(elf.symbol_table->st_info))

#include "interpret_symbol_table.h"