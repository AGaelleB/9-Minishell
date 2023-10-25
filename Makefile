# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/17 10:44:39 by abonnefo          #+#    #+#              #
#    Updated: 2023/10/25 17:40:21 by abonnefo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g3

OBJ_DIR_MINISHELL = objs_minishell

OBJ_DIR_LIBFT = libft/obj_libft

SRCS_EXECUTION = srcs/main_minishell.c \
	srcs/execution/execve.c \
	srcs/execution/handle_quotes.c \
	srcs/execution/read_fd.c \

SRCS_BUILTINS = srcs/builtins/all_builtins_verifs.c \
	srcs/builtins/builtin_cd.c \
	srcs/builtins/builtin_echo.c \
	srcs/builtins/builtin_env.c \
	srcs/builtins/builtin_export.c \
	srcs/builtins/builtin_pwd.c \
	srcs/builtins/builtin_unset.c \
	srcs/builtins/builtins_exit.c \
	srcs/builtins/check_builtins_1.c \
	srcs/builtins/check_builtins_2.c \
	srcs/builtins/exec_builtins.c \

SRCS_INIT_AND_PARSING = srcs/init_and_parsing/alloc_and_cpy_parse_echo.c \
	srcs/init_and_parsing/count_args_quotes.c \
	srcs/init_and_parsing/create_cmd.c \
	srcs/init_and_parsing/handle_token.c \
	srcs/init_and_parsing/init_token.c \
	srcs/init_and_parsing/init_quote.c \
	srcs/init_and_parsing/path_token.c \
	srcs/init_and_parsing/path.c \
	srcs/init_and_parsing/parsing_echo.c \
	srcs/init_and_parsing/parse_spaces_redir.c \

SRCS_REDIRECTIONS = srcs/redirections/epur_filename.c \
	srcs/redirections/redirect_at_beginning.c \
	srcs/redirections/redirect_file_in_and_out.c \
	srcs/redirections/redirect_file_in_out.c \
	srcs/redirections/redirect_heredoc.c \
	srcs/redirections/redirect_open_fd.c \

SRCS_SIGNALS = srcs/signals/signal_exit.c \

SRCS_UTILS = srcs/utils/check_syntax_caracteres.c \
	srcs/utils/check_syntax_pipes.c \
	srcs/utils/error.c \
	srcs/utils/free_bis.c \
	srcs/utils/free.c \
	srcs/utils/libft_modify.c \
	srcs/utils/split_command_on_pipe.c \
	srcs/utils/split_string_token_utils.c \
	srcs/utils/split_string_token.c \
	srcs/utils/split_string.c \
	srcs/utils/utils.c \

SRCS_LIBFT = libft/ft_atoi.c \
	libft/ft_bzero.c \
	libft/ft_calloc.c \
	libft/ft_isalnum.c \
	libft/ft_isalpha.c \
	libft/ft_isascii.c \
	libft/ft_isdigit.c \
	libft/ft_isprint.c \
	libft/ft_itoa.c \
	libft/ft_memchr.c \
	libft/ft_memcmp.c \
	libft/ft_memcpy.c \
	libft/ft_memmove.c \
	libft/ft_memset.c \
	libft/ft_putchar_fd.c \
	libft/ft_putendl_fd.c \
	libft/ft_putnbr_fd.c \
	libft/ft_putstr_fd.c \
	libft/ft_split.c \
	libft/ft_strchr.c \
	libft/ft_strdup.c \
	libft/ft_striteri.c \
	libft/ft_strjoin.c \
	libft/ft_strlcat.c \
	libft/ft_strlcpy.c \
	libft/ft_strlen.c \
	libft/ft_strmapi.c \
	libft/ft_strncmp.c \
	libft/ft_strnstr.c \
	libft/ft_strrchr.c \
	libft/ft_strtrim.c \
	libft/ft_substr.c \
	libft/ft_tolower.c \
	libft/ft_toupper.c \

SRCS_LIBFT_BONUS = libft/ft_lstadd_back.c \
	libft/ft_lstadd_front.c \
	libft/ft_lstclear.c \
	libft/ft_lstdelone.c \
	libft/ft_lstiter.c \
	libft/ft_lstlast.c \
	libft/ft_lstmap.c \
	libft/ft_lstnew.c \
	libft/ft_lstsize.c \
	libft/ft_realloc.c \

SRCS_GNL = libft/GNL/get_next_line.c \
	libft/GNL/get_next_line_utils.c \

SRCS_PRINTF = libft/ft_printf/ft_printf.c \
	libft/ft_printf/ft_print_%.c \
	libft/ft_printf/ft_print_c.c \
	libft/ft_printf/ft_print_d.c \
	libft/ft_printf/ft_print_p.c \
	libft/ft_printf/ft_print_s.c \
	libft/ft_printf/ft_print_u.c \
	libft/ft_printf/ft_print_x.c \

OBJS = $(SRCS_EXECUTION:%.c=$(OBJ_DIR_MINISHELL)/%.o) \
		$(SRCS_BUILTINS:%.c=$(OBJ_DIR_MINISHELL)/%.o) \
		$(SRCS_INIT_AND_PARSING:%.c=$(OBJ_DIR_MINISHELL)/%.o) \
		$(SRCS_REDIRECTIONS:%.c=$(OBJ_DIR_MINISHELL)/%.o) \
		$(SRCS_SIGNALS:%.c=$(OBJ_DIR_MINISHELL)/%.o) \
		$(SRCS_UTILS:%.c=$(OBJ_DIR_MINISHELL)/%.o) \
		$(SRCS_LIBFT:%.c=$(OBJ_DIR_LIBFT)/%.o) \
		$(SRCS_LIBFT_BONUS:%.c=$(OBJ_DIR_LIBFT)/%.o) \
		$(SRCS_GNL:%.c=$(OBJ_DIR_LIBFT)/%.o) \
		$(SRCS_PRINTF:%.c=$(OBJ_DIR_LIBFT)/%.o) \

AR = ar rcs

RM = rm -f

$(OBJ_DIR_MINISHELL)/%.o $(OBJ_DIR_LIBFT)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS)  -lreadline -lhistory -o $(NAME)
	@echo "\033[5;36m\n-gcc *.c libft done\033[0m"
	@echo "\033[5;36m-gcc *.c get_next_line done\033[0m"
	@echo "\033[5;36m-gcc *.c ft_printf done\033[0m"
	@echo "\033[5;36m-gcc *.c minishell done\n\033[0m"
	@echo "\033[1;32m[Make : 'minishell' is done]\033[0m"

all : $(NAME)

clean :
	@$(RM) $(OBJS)
	@echo "\033[1;33m[.o] Object files removed\033[0m"

fclean : clean
	@$(RM) $(NAME)
	@echo "\033[1;33m[.a] Binary file removed\033[0m"

re : fclean all

.PHONY: all clean fclean re
