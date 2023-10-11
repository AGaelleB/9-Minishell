/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/11 17:29:31 by abonnefo         ###   ########.fr       */
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


typedef struct s_command			t_command;

typedef struct s_parser
{
	char	**args;
	int		i;
	int		idx;
	bool	in_quote;
} t_parser;


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
	TYPE_FILENAME, // 11			

} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
	char				*command;

} t_token;


typedef struct s_split_token
{
	int					i;
	int					j;
	int					start;
	int					delim_len;
	int					len_to_copy;
} t_split_token;

typedef struct s_tokenizer
{
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	char		**words;
	char		*delimiters[6];
	int			i;
	int			state;
	bool		flag_single_quote;
	bool		flag_double_quote;
} t_tokenizer;

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

typedef struct s_process_data
{
	t_command	*current;
	t_command	*command;  // ajouté ici
	pid_t		*child_pids;
	pid_t		pid;
	char		**envp;
	int			infile;  // déplacé ici et non plus un pointeur
	int			index;  // ajouté ici
} t_process_data;

void	print_commands_and_tokens(t_command *head);
void	ft_set_args_and_paths(t_command *current, char **envp);
char	**parse_input_quote(char *input);

void	ft_all_builtins_verif(t_command *current);
char	**split_string_token(char *str, char **delimiters);

// char *extract_filename(const char *input);


/***********MAIN***********/


/***********BUILTINS***********/
int			ft_builtin_write_exit(char *input);
void		ft_builtin_ctrl_d(char *input);
int			ft_all_builtins(char *input);
int			ft_builtin_enter(char *input);
int			ft_is_all_space(char *input);

int			ft_builtin_echo_fd(char **tab);
int			ft_builtin_pwd_fd(int fd);

/***********EXECUTION***********/
t_command	*create_new_cmd(char *command_str, char **envp);
t_command	*append_new_cmd(t_command **head, t_command *new_cmd);
t_command	*get_command(char *input, char **envp);

void		init_execve(t_command *cur, pid_t **childs_pids);
int			execve_process(t_command *current, char **envp);


/***********INIT_AND_PARSING***********/
int			count_args_single_quotes(char *input);
int			countcount_args_double_quotes_args(char *input);

t_token		*handle_cmd_token(t_tokenizer *tz, char **envp);
t_token		*handle_redir_tokens(t_tokenizer *tz);
t_token		*handle_arg_token(t_tokenizer *tz);
t_token		*handle_single_quote_token(t_tokenizer *tz);
t_token		*handle_double_quote_token(t_tokenizer *tz);


int			verif_nb_quote(char *input);
bool		contains_single_quote(char *str);
bool		contains_double_quote(char *str);

t_token		*new_token(t_e_token_type e_type, char *split_value);
t_token 	*tokenize_input(char *input, char **envp);
char		*ft_check_paths(char **envp, char *args);
void		execve_fd(t_command *current, char **envp);

int			calculate_new_len(char *input);
char		*copy_with_spaces(char *input, char *new_input);
char		*add_spaces_around_redirections(char *input);


/***********REDIRECTIONS***********/
char		*epur_filename(t_token *token_head);

int			is_redir_at_beginning(char *input, int i);

int			redirect_file_in(t_command *current, t_token *token, t_token *token_head);
int			redirect_file_out(t_command *current, t_token *token, t_token *token_head);
int			redirect_append_file_out(t_command *current, t_token *token, t_token *token_head);
int			is_redir_at_beginning(char *input, int i);

int			aleatori_char(void);
char		*create_file_name(void);
int			write_in_fd(int fd, char *delimiter);
int			redirect_heredoc(t_command *current, t_token *token);

int			open_fd(t_command *command);


/***********SIGNALS***********/
void		ft_builtin_ctrl_c (int sig);
void		block_signal(int signal);


/***********UTILS***********/
int			check_valid_caractere_filename(char c);
int			check_syntax_errors(char *input);
int			pipe_syntax_errors(char *input);


void		ft_print_error(char *str);
void		exit_with_error(char *message, pid_t *child_pids);
// int			check_valid_identifier(char c);


void		ft_free_tab(char **tab);
void		ft_free_struct(t_command *current, t_token *head);
void		ft_free_current(t_command *current);
void		free_file_name(char *file_name);
void		cleanup(pid_t *child_pids, int infile);

void		free_tokens(char **tokens, int num);
char		**free_token_split(char **tokens);

int			ft_strchr_slash(char *str, char c);
int			ft_strncmp_minishell(char *s1, char *s2, int n);
int			ft_strcmp_minishell(char *s1, char *s2);
char		*ft_strjoin_minishell(char *s1, char *s2);
// char		*ft_strstr(const char *str, const char *substr);


int			is_delimiter(char *str, char **delimiters, int *delim_len);
int			count_split_tokens_str(char *str, char **delimiters);
char		**allocate_tokens(int token_count);
char		*allocate_and_cpy(char *start, int size);

void		count_and_set_pipes(char *input, t_command *command);
void		ft_close_fd(void);
int			is_empty_or_space(char *str);
int			ft_isspace(int c);

char		**split_string(const char *str, char delimiter);

#endif