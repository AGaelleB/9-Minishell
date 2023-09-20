/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/20 14:05:25 by bfresque         ###   ########.fr       */
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


typedef enum e_token_type
{
	TYPE_CMD, // 0				// cat 			de cat celine.txt
	TYPE_ARG, // 1				// celine.txt 	de cat celine.txt
	TYPE_SEPARATOR, // 2		// "|" 
	TYPE_REDIR_OUT, // 3		// ">"
	TYPE_REDIR_IN, // 4			// "<"
	TYPE_REDIR_APPEND, // 5		// ">>"
	TYPE_DELIMITATOR, // 6		// "<<"
	TYPE_F_OUT, // 7
	TYPE_F_IN, // 8

	// TYPE_BUILTIN,
	// TYPE_HEREDOC,
	// TYPE_EOF
} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
	struct s_token		*before;
} t_token;

typedef struct s_sigset
{

} t_sigset;

typedef struct s_command
{
	int					nb_pipes;
	char				*command;	// e.g. "cat test.txt"
	char				**command_arg;	// e.g. "cat"
	char				*command_path;	// e.g. /usr/bin/cat/
	int					fd[2];
	int					fd_out;
	int					fd_in; // NEW
	struct s_token		*token_head; // Pointeur vers le premier token de cette commande
	struct s_command	*next;	// Pointeur vers la commande suivante
} t_command;



// typedef struct s_command
// {
// 	int					nb_pipes;
// 	char				*command;	// e.g. "cat test.txt"
// 	char				**command_arg;	// e.g. "cat"
// 	char				*command_path;	// e.g. /usr/bin/cat/
// 	int					fd[2];
// 	int					fd_out;
// 	int					fd_in; // NEW
// 	struct s_token		*token;
// 	struct s_command	*next;	// Pointeur vers la commande suivante
// } t_command;



void print_commands_and_tokens(t_command *head);
void	ft_set_args_and_paths(t_command *current, char **envp);


/***********MAIN***********/


/***********BUILTINS***********/
int			ft_builtin_write_exit(char *input);
void		ft_builtin_ctrl_D(char *input);
int			ft_all_builtins_exit(char *input);
int			ft_builtin_enter(char *input);
int			ft_is_all_space(char *input);


/***********EXECUTION***********/
t_command	*get_command(char *input);
int			child_process(t_command *current, char **envp);



/***********INIT_AND_PARSING***********/
t_token		*tokenize_input(char *input);
char		*ft_check_paths(char **envp, char *args);
void		execve_fd(t_command *current, char **envp);

// void execve_fd(t_command *current, t_e_token_type e_type, char **envp);



/***********SIGNALS***********/
void		ft_signal_ctrl_C (int sig);
void		block_signal(int signal);



/***********UTILS***********/
void		ft_print_error(char *str);

void		ft_free_tab(char **tab);
void		ft_free_all_cmd(t_command *command);
void		ft_free_tokens(t_token *head);
void		ft_free_current(t_command *current);

int			ft_strchr_slash(char *str, char c);
int			ft_strncmp_minishell(char *s1, char *s2, int n);
int			ft_strcmp_minishell(char *s1, char *s2);
char		*ft_strjoin_minishell(char *s1, char *s2);
char		**split_string(const char *str, char delimiter);

void		count_and_set_pipes(char *input, t_command *command);
void		ft_close_fd();
int			is_empty_or_space(char *str);


#endif