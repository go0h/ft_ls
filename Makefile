# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: astripeb <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/07 15:14:49 by pcredibl          #+#    #+#              #
#    Updated: 2020/06/13 15:48:28 by astripeb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

GREEN 				:= \033[0;32m
RED 				:= \033[0;31m
RESET				:= \033[0m

NAME				:= ft_ls

#COMPILER
CC 					:= gcc

#LIBFT
LIB_DIR				:= ./libft
LIBFT				:= libft.a

#PROJECT_DIRS
INC_DIR				:= ./includes
OBJ_DIR				:= ./obj
SRC_DIR				:= ./srcs

#COMPILER FLAGS
CFLAGS				?= -Wall -Wextra -Werror
LFLAGS				:= -I $(LIB_DIR)/includes -I $(INC_DIR)
LIB					:= -L $(LIB_DIR) -lft
DEPEND				:= -MD -MT


################################################################################
#									SOURCE FILES					 		   #
################################################################################

SRC					:= main.c options.c dir_funcs.c ft_exit.c\
					functors.c short_print.c long_print.c read_file_stat.c

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
	$(CC) $(CFLAGS) $(LFLAGS) $(addprefix $(OBJ_DIR)/, $(OBJ)) $(LIB) -o $@
	echo "$(GREEN)$@ was created ✅$(RESET)"

$(OBJ):%.o:%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(OBJ_DIR)/$@ -c $< $(DEPEND) $@
	echo "$(GREEN)$@ was created$(RESET)"

lib:
	$(MAKE) -C $(LIB_DIR)

$(OBJ_DIR):
	mkdir -p $@

Debug:
	$(MAKE) CFLAGS="-g"

clean:
	$(MAKE) clean -C $(LIB_DIR)
	rm -rf $(OBJ_DIR)
	echo "$(RED)objs files was deleted$(RESET)"

fclean: clean
	$(MAKE) fclean -C $(LIB_DIR)
	echo "$(RED)$(LIBFT) was deleted$(RESET)"
	rm -rf $(NAME)
	echo "$(RED)$(NAME) was deleted$(RESET)"

re: fclean all

include $(wildcard $(OBJ_DIR)/*.d)

.SILENT: all clean lib Debug re fclean $(NAME) $(OBJ) $(OBJ_DIR)

.PHONY: clean fclean re all Debug
