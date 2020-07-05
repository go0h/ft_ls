# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: astripeb <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/07 15:14:49 by pcredibl          #+#    #+#              #
#    Updated: 2020/07/05 12:09:40 by astripeb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN 				:= \033[0;32m
RED 				:= \033[0;31m
RESET				:= \033[0m
BLINK				:= \033[5m
NORMAL				:= \033[25m
BOLDU				:= \033[1m\033[4m

NAME				:= ft_ls

#COMPILER
CC 					:= gcc

#LIBFT
LIB_DIR				:= ./libft
LIBFT				:= libft.a

#PROJECT_DIRS
INC_DIR				:= ./includes
OBJ_DIR				:= ./.obj
SRC_DIR				:= ./srcs

#COMPILER FLAGS
CFLAGS				?= -Wall -Wextra -Werror -O3
LFLAGS				:= -I $(LIB_DIR)/includes -I $(INC_DIR)
LIB					:= -L $(LIB_DIR) -lft
DEPEND				:= -MD -MT


################################################################################
#									SOURCE FILES					 		   #
################################################################################

SRC					:= ls_main.c ls_options.c ls_dir_funcs.c ls_utils.c\
					ls_functors.c ls_short_print.c ls_long_print.c\
					ls_read_file_stat.c ls_print_files.c

################################################################################
#																	 		   #
################################################################################

OBJ					:= $(SRC:.c=.o)

vpath %.c $(SRC_DIR)
vpath %.o $(OBJ_DIR)
vpath %.h $(INC_DIR)
vpath %.a $(LIB_DIR)

all: lib $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(LFLAGS) $(addprefix $(OBJ_DIR)/, $(OBJ)) -o $@ $(LIB)
	echo "$(GREEN)Creating $(BOLDU)$@$(RESET)"
	echo "$(GREEN)$(BLINK)DONE✅$(NORMAL)$(RESET)"

$(OBJ):%.o:%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $(OBJ_DIR)/$@ $(DEPEND) $@
	echo "$(GREEN)Creating $(BOLDU)$@$(RESET)"

lib:
	$(MAKE) -C $(LIB_DIR)

$(OBJ_DIR):
	mkdir -p $@

Debug:
	$(MAKE) CFLAGS="-g3"

clean:
	$(MAKE) clean -C $(LIB_DIR)
	rm -rf $(OBJ_DIR)
	echo "$(RED)Deleting $(BOLDU)objs files$(RESET)"

fclean: clean
	$(MAKE) fclean -C $(LIB_DIR)
	echo "$(RED)Deleting $(BOLDU)$(LIBFT)$(RESET)"
	rm -rf $(NAME)
	echo "$(RED)Deleting $(BOLDU)$(NAME)$(RESET)"

re: fclean all

include $(wildcard $(OBJ_DIR)/*.d)

.SILENT: all clean lib Debug re fclean $(NAME) $(OBJ) $(OBJ_DIR)

.PHONY: clean fclean re all Debug
