/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 14:37:21 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <stdint.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>

# include "../includes/ft_printf.h"
# include "../includes/get_next_line.h"
# include "../includes/libft.h"

# define RESET "\033[0m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# define SINGLE_QUOTE	'\''
# define DOUBLE_QUOTE	'\"'

# define SUCCESS	0
# define FAILURE	1

typedef struct s_quote 
{
	char	*str;
	char	**args;
	struct	s_quote  *next;
} t_quote;

typedef enum e_token_type
{
	TYPE_CMD, // 0				// cat 			de cat celine.txt
	TYPE_ARG, // 1				// celine.txt 	de cat celine.txt
	TYPE_SEPARATOR, // 2		// "|" 
	TYPE_SINGLE_QUOTE, // 3		// ' '
	TYPE_REDIR_OUT, // 4		// ">"
	TYPE_REDIR_IN, // 5			// "<"
	TYPE_REDIR_APPEND, // 6		// ">>"
	TYPE_HEREDOC, // 7			// "<<"
	TYPE_F_OUT, // 8
	TYPE_F_IN, // 9
	TYPE_EOF, // 10				// eof
} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
} t_token;

typedef struct s_command
{
	int					nb_pipes;
	char				*command;	// e.g. "cat test.txt"
	char				**command_arg;	// e.g. "cat"
	char				*command_path;	// e.g. /usr/bin/cat/
	int					fd[2];
	int					fd_out;
	int					fd_in;
	char				*file_name;
	struct s_token		*token_head;
	struct s_quote		*quote_head;
	struct s_command	*next;
} t_command;


void	print_commands_and_tokens(t_command *head);
void	ft_set_args_and_paths(t_command *current, char **envp);
char	**parse_input_quote(char *input);

void	print_t_quote(t_quote *quote);
void	ft_all_builtins_verif(t_command *current);
char	**split_string_token(char *str, char **delimiters);


/***********MAIN***********/


/***********BUILTINS***********/
int			ft_builtin_write_exit(char *input);
void		ft_builtin_ctrl_D(char *input);
int			ft_all_builtins(char *input);
int			ft_builtin_enter(char *input);
int			ft_is_all_space(char *input);

int			ft_builtin_echo_fd(char **tab);
int			ft_builtin_pwd_fd(int fd);

/***********EXECUTION***********/
// t_command	*get_command(char *input);
t_command	*get_command(char *input, char **envp);
void		ft_append_str(char **original, char *new_str);
int			child_process(t_command *current, char **envp);



/***********INIT_AND_PARSING***********/
// t_token		*tokenize_input(char *input);
t_token 	*tokenize_input(char *input, char **envp);
char		*ft_check_paths(char **envp, char *args);
void		execve_fd(t_command *current, char **envp);

int			verif_nb_single_quote(char *str);

/***********REDIRECTIONS***********/
int			redirect_file_in(t_command *current, t_token *token);
int			redirect_file_out(t_command *current, t_token *token);
int			redirect_append_file_out(t_command *current, t_token *token);

int			aleatori_char(void);
char		*create_file_name(void);
int			write_in_fd(int fd, char *delimiter);
int			redirect_heredoc(t_command *current, t_token *token);

int			open_fd(t_command *command);



/***********SIGNALS***********/
void		ft_signal_ctrl_C (int sig);
void		block_signal(int signal);


/***********UTILS***********/
void		ft_print_error(char *str);

void		ft_free_tab(char **tab);
void		ft_free_current(t_command *current);
void		free_file_name(char *file_name);
void		ft_free_tokens(t_token *head);
void		ft_free_struct(t_command *current, t_token *head);

int			ft_strchr_slash(char *str, char c);
int			ft_strncmp_minishell(char *s1, char *s2, int n);
int			ft_strcmp_minishell(char *s1, char *s2);
char		*ft_strjoin_minishell(char *s1, char *s2);
char		**split_string(const char *str, char delimiter);

void		count_and_set_pipes(char *input, t_command *command);
void		ft_close_fd();
int			is_empty_or_space(char *str);
int			ft_isspace(int c);

#endif