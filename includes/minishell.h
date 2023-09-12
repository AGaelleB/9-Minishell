/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/12 13:33:27 by abonnefo         ###   ########.fr       */
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

typedef enum e_token_type
{
	TYPE_CMD,
	TYPE_ARG,
	TYPE_SEPARATOR,
	TYPE_REDIR_OUT,		// ">"
	TYPE_REDIR_IN,		// "<"
	TYPE_REDIR_APPEND,	// ">>"
	TYPE_BUILTIN,
	TYPE_HEREDOC,
	TYPE_EOF
} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
} t_token;

typedef struct s_sigset
{

} t_sigset;



typedef struct s_command
{
	int					nb_pipes; //ATTENTION
	char				*command;			// e.g. "cat test.txt"
	char				**command_arg;		// e.g. "cat"
	char				*command_path;		// e.g. /usr/bin/cat/
	// char	separator;			// e.g. '|'
	int					read_fd;
	int					write_fd;
	struct s_token		*token;
	struct	s_command	*next;	// Pointer vers la prochaine commande
} t_command;


/***********BUILTINS***********/
int			ft_builtin_exit(char *input);
int			ft_all_builtins(char *input);



/***********EXECUTION***********/
t_command	*get_command(char *input);
void		child_process(t_command *current, int read_fd, int write_fd, char **envp);



/***********INIT_AND_PARSING***********/
t_token		*tokenize_input(char *input);
char		*ft_check_paths(char **envp, char *args);
void		open_fd(t_command *current, char **envp);



/***********SIGNALS***********/
void		handle_sigint (int sig);
void	block_signal(int signal);



/***********UTILS***********/
void		ft_print_error(char *str);
void		ft_free_tab(char **tab);
void		ft_free_all_cmd(t_command *command);
int			ft_strchr_slash(char *str, char c);
int			ft_strncmp_minishell(char *s1, char *s2, int n);
int			ft_strcmp_minishell(char *s1, char *s2);
char		*ft_strjoin_minishell(char *s1, char *s2);
char		**split_string(const char *str, char delimiter);
void		count_and_set_pipes(char *input, t_command *command);

#endif