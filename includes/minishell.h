/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 14:36:09 by bfresque         ###   ########.fr       */
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

# define SIZE 1000

extern int	g_exit_status;

typedef struct s_command			t_command;

typedef struct s_env
{
	char				**cpy_env;
	char				*str;
	bool				flag_error;
	int					pwd_index;
	int					oldpwd_index;
	char				*path_to_change;
	char				*new_directory;
	struct s_env		*next;
} t_env;

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
	char				*split_heredoc;
	struct s_token		*next;
	struct s_token		*prev;
	char				*command;
	char				*command_two;
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
	char		**heredocs; // TEST
	char		*delimiters[6];
	int			i;
	int			state;
	bool		cmd_processed;
	bool		flag_single_quote;
	bool		flag_double_quote;
} t_tokenizer;

typedef struct s_arg_handler
{
	t_env	*env;
	char	*input;
	int		*i;
	char	*arg;
	int		*arg_idx;
} t_arg_handler;

typedef struct s_process_data
{
	t_command	*current;
	t_command	*command;
	pid_t		*child_pids;
	pid_t		*heredoc_fd;
	pid_t		pid;
	char		**envp;
	int			infile;
	int			index;
} t_process_data;

typedef struct s_command
{
	int					nb_pipes;
	char				*command;	// e.g. "cat test.txt"
	char				**command_arg;	// e.g. "cat"
	char				**export_arg;
	char				*command_path;	// e.g. /usr/bin/cat/
	int					fd[2];
	int					fd_out;
	int					fd_in;
	char				*file_name;
	char				*heredoc;  // PAS FREE
	char				**heredocs;  // tableau dynamique pour stocker les noms de fichiers heredoc
	int					flag_chevron;
	bool				last_redir_is_heredoc; // Tableau pour les flags actifs des heredocs
	int					exit_status;
	// char				*split_herdoc;
	struct s_token		*token_head;
	struct s_quote		*quote_head;
	struct s_command	*next;
} t_command;

void	ft_close_all_fd(void);
void ft_free_all(t_command *current, t_token *token);
void ft_free_env(t_env *env);
void	print_commands_and_tokens(t_command *head);
void	ft_set_args_and_paths(t_command *cur, t_env *env);
char	**parse_input_quote(char *input);

int		builtins_verif(t_command *current, t_env	*env_bis);
char	**split_string_token(char *str, char **delimiters);
t_token *handle_multiple_heredocs(t_command *current, t_token *token); // NEWWWW
// pid_t heredoc_open_fd_pipe(t_command *command, t_token **token);
int heredoc_open_fd_pipe(t_command *command, t_token **token);

int	find_env_var(t_env *env, char *arg);

// char *extract_filename(const char *input);


/***********MAIN***********/


/***********BUILTINS***********/
// void		ft_builtin_cd(char **args, t_env *env);
void		update_env_pwd(t_env *env, char *new_pwd);
int			ft_builtin_cd(char **args, t_env *env);

int			ft_builtin_write_exit(char *input);
void		ft_builtin_ctrl_d(char *input);
int			error_input(char *input);
int			ft_builtin_enter(char *input);
int			ft_is_all_space(char *input);

void		ft_builtin_env(t_env	*env);
void		copy_env(t_env *env, char **envp);

int			ft_builtin_export(char **args, t_env *env);

int			ft_builtin_echo(char **tab);
int			ft_builtin_pwd(int fd);

void		ft_builtin_unset(char **args, t_env *env);

int			check_pwd(t_command *current);
int			check_echo(t_command *current, t_env *env);
int			check_cd(t_command *current);

int			check_env(t_command *current, t_env *env);
int			check_unset(t_command *current);
int			check_export(t_command *current, t_env *env);

void		execve_builtins_unset_export(t_command *cmd, t_env *env_bis);
void		execve_builtin_cd(t_command *cmd, t_env *env_bis);


/***********EXECUTION***********/
t_command	*create_new_cmd(char *command_str, t_env *env);
t_command	*append_new_cmd(t_command **head, t_command *new_cmd);
t_command	*get_command(char *input, t_env *env);

void	handle_arg_value(t_env *env, char *input, int *i, char *arg, int *arg_idx);

void	handle_all_process(t_process_data *data, t_env *env);

void		init_execve(t_command *cur, pid_t **childs_pids);
int			execve_process(t_command *cur, t_env *env);


/***********INIT_AND_PARSING***********/
char		*ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx);

int			count_args_single_quotes(char *input);
int			count_args_double_quotes_args(char *input);

t_token		*handle_cmd_token(t_tokenizer *tz, t_env *env);
t_token		*handle_redir_tokens(t_tokenizer *tz);
t_token		*handle_arg_token(t_tokenizer *tz);
t_token		*handle_single_quote_token(t_tokenizer *tz);
t_token		*handle_double_quote_token(t_tokenizer *tz);

int			verif_nb_quote(char *input);
bool		contains_single_quote(char *str);
bool		contains_double_quote(char *str);

char	*env_var_exists(t_env *env, char *var);
char	*get_env_value(t_env *env, char *str);
void	append_env_value_to_arg(char *value, char *arg, int *arg_idx);

t_token		*new_token(t_e_token_type e_type, char *split_value);
// t_token		*new_token(t_e_token_type e_type, char *split_value, char *split_heredoc);
t_token 	*tokenize_input(char *input, t_env *env);
char		*ft_check_paths_token(t_env *env, char *args);
char		*ft_check_paths(t_env *env, char *args);
void		execve_fd(t_command *current, t_env *env);

char		**parse_arg_export(char *input);

char		**parse_input_quote_echo(t_env *env, char *input);
void		skip_spaces_echo(char *input, int *i);
void		handle_quotes_echo(char *input, int *i, bool *double_quote, bool *single_quote);

int			calculate_new_len(char *input);
char		*copy_with_spaces(char *input, char *new_input);
char		*add_spaces_around_redir(char *input);


/***********REDIRECTIONS***********/
char		*epur_filename(t_token *token_head);

char		*epur_filename_heredoc(t_token *token_head);

int			is_redir_at_beginning(char *input, int i);

int			redirect_file_in(t_command *current, t_token *token, t_token *token_head);
int			redirect_file_out(t_command *current, t_token *token, t_token *token_head);
int			redirect_append_file_out(t_command *current, t_token *token, t_token *token_head);
int			is_redir_at_beginning(char *input, int i);

int			aleatori_char(void);
// char		*create_file_name(void);
// char		*create_file_name(t_command *command);

void	heredoc_open_fd(t_command *command, t_token **token);
void	redirect_file_in_open_fd(t_command *command, t_token *token, t_token *token_head);
void	redirect_file_out_open_fd(t_command *command, t_token *token, t_token *token_head);
void	redirect_append_file_out_open_fd(t_command *command, t_token *token, t_token *token_head);

// int			write_in_fd(int fd, char *delimiter);
// int			write_in_fd(int fd, char *delimiter, char *file_name);
int			redirect_heredoc(t_command *current, t_token *token);

int			open_fd(t_command *command);

int			write_in_fd(int fd, char *delimiter, t_command *current);

/***********SIGNALS***********/
void		ft_builtin_ctrl_c (int sig);
void		handle_signals_heredoc(void);
int			ctrl_d_heredoc(char *input, int i, char *delimiter);


/***********UTILS***********/
int			is_redirection(char c);
void		ft_skip_redirection_and_file(char *input, int *i);

int			check_valid_caractere_filename(char c);
int			check_syntax_errors(char *input);
int			pipe_syntax_errors(char *input);


void		ft_print_error(char *str);
void		exit_with_error(char *message, pid_t *child_pids);
void		print_error_cd(t_env *env, int i);
int			verif_access(t_command *current, char *command);
// int			check_valid_identifier(char c);

char	*create_heredoc(void);

int			count_args(char *input, int i);
int			count_arg_length(char *input, int i);


void		ft_free_tab(char **tab);
void		ft_free_token(t_command *current);
void		ft_free_current(t_command *current);
void		clean_heredoc_files(t_command *cur);

void		cleanup(pid_t *child_pids, int infile);

void		free_tokens(char **tokens, int num);
char		**free_token_split(char **tokens);

int			ft_strchr_slash(char *str, char c);
int			ft_strncmp_minishell(char *s1, char *s2, int n);
int			ft_strcmp_minishell(char *s1, char *s2);
char		*ft_strjoin_minishell(char *s1, char *s2);
// char		*ft_strstr(const char *str, const char *substr);

char		**split_command_on_pipe(char *input);

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