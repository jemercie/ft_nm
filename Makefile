NAME			=	ft_nm
FLAGS			=	-Wall -Wextra -Werror
SRC				=	src/main.c								\
					src/open_and_map_file.c					\
					src/find_and_print_symbol_table_x64.c	\

OBJ				=	$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_DIR			=	src
OBJ_DIR			=	obj
INCLUDE_DIR		=	-I include

$(NAME): $(OBJ)
	@clang  $(OBJ) $(INCLUDE_DIR) $(FLAGS) -o $(NAME) -lm
	@echo "$(PURPLE)compilation finished$(NC)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@clang $(FLAGS) $(INCLUDE_DIR) -c $< -o $@
	@echo "$(BLUE)creating object for:$(NC) $(notdir $<)"

all:
	$(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(PINK)$(NAME) objects removed$(NC)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(PINK)$(NAME) binary removed$(NC)"

re: fclean $(NAME)

.PHONY = all clean fclean re

#########  COLORS  ##########

BLUE = \e[38;2;102;178;255m
PURPLE = \e[38;2;147;112;219m
PINK = \e[38;2;255;182;193m
NC = \033[0;37m

