
#define Elfw(x) Elf32_##x
#define ELFF(func_name) func_name##_x32
#define PADDING_LEN                          11
#define ST_TYPE(symbol_table)  (ELF32_ST_TYPE(symbol_table->st_info))
#define ST_BIND(symbol_table)  (ELF32_ST_BIND(symbol_table->st_info))
#define SYMBOL_TYPE(elf)       (ELF32_ST_TYPE(elf.symbol_table->st_info))

#include "interpret_symbol_table.h"