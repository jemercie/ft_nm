NAME			=	nm
FLAGS			=	-Wall -Wextra -Werror
SRC				=	src/main.c

OBJ				=	$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
SRC_DIR			=	src
OBJ_DIR			=	obj
INCLUDE_DIR		=	-I include



$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@clang $(FLAGS) $(INCLUDE_DIR) -c $< -o $@
	@echo "$(YELLOW)creating object for:$(NC) $(notdir $<)"


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
PURPLE = \e[38;2;153;153;255m
PINK = \e[38;2;255;153;204m
YELLOW = \e[38;2;255;205;0m
NC = \033[0;37m

