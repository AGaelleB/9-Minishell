/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/27 16:22:37 by abonnefo         ###   ########.fr       */
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
# include <stdbool.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

# include "../includes/ft_printf.h"
# include "../includes/get_next_line.h"
# include "../includes/libft.h"

# define RESET "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"

# define SIZE 1000

extern int					g_exit_status;
typedef struct s_command	t_command;

typedef struct s_env
{
	char			**cpy_env;
	char			*str;
	bool			flag_error;
	int				pwd_index;
	int				oldpwd_index;
	char			*path_to_change;
	char			*new_directory;
	struct s_env	*next;
}	t_env;

// typedef struct s_parser
// {
// 	char	**args;
// 	int		i;
// 	int		idx;
// 	bool	in_quote;
// }	t_parser;

typedef struct s_parser
{
    char    **args;
    int     i;
    int     idx;
    bool    in_quote;
    t_env   *env;
    char    *input;
    char    *arg;
    int     arg_idx;
    bool    double_quote;
    bool    single_quote;
}   t_parser;


typedef struct s_quote
{
	char			*str;
	char			**args;
	struct s_quote	*next;
}	t_quote;

typedef enum e_token_type
{
	TYPE_CMD,
	TYPE_ARG,
	TYPE_SEPARATOR,
	TYPE_SINGLE_QUOTE,
	TYPE_REDIR_OUT,
	TYPE_REDIR_IN,
	TYPE_REDIR_APPEND,
	TYPE_HEREDOC, //7
	TYPE_F_OUT,
	TYPE_F_IN,
	TYPE_EOF, // 10
	TYPE_FILENAME,
}	t_e_token_type;

typedef struct s_token
{
	t_e_token_type	type;
	char			*split_value;
	char			*split_heredoc;
	struct s_token	*next;
	struct s_token	*prev;
	char			*command;
	char			*command_two;
}	t_token;

typedef struct s_split_token
{
	int		i;
	int		j;
	int		start;
	int		delim_len;
	int		len_to_copy;
}	t_split_token;

typedef struct s_tokenizer
{
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	char		**words;
	char		**heredocs;
	char		*delimiters[6];
	int			i;
	int			state;
	bool		cmd_processed;
	bool		flag_single_quote;
	bool		flag_double_quote;
}	t_tokenizer;

typedef struct s_arg_handler
{
	t_env	*env;
	char	*input;
	int		*i;
	char	*arg;
	int		*arg_idx;
	bool	*double_quote;
	bool	*single_quote;
}	t_arg_handler;

typedef struct s_here_doc
{
	int	fd[2];
}	t_here_doc;

typedef struct s_export
{
	char	*ret;
	char	*new;
	int		flag;
	int		i;
	int		j;
	int		k;
	int		l;
	int		m;
}	t_export;

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
	t_here_doc	*heredocs;
	int			current_hd;
	int			count_hd;
}	t_process_data;

typedef struct s_command
{
	int					nb_pipes;
	char				*command;
	char				**command_arg_main;
	char				**command_arg;
	char				**export_arg;
	char				*command_path;
	int					fd[2];
	int					fd_out;
	int					fd_in;
	char				*file_name;
	char				*heredoc; // PAS FREE
	char				**heredocs;
	int					flag_chevron;
	bool				last_redir_is_heredoc;
	int					exit_status;
	int					flag;
	struct s_token		*token_head;
	struct s_quote		*quote_head;
	struct s_command	*next;
}	t_command;

int	execve_process(t_process_data *data, t_env *env);
int	verif_access(t_process_data *data, t_env *env, char *command);
void	write_exit_simple(t_env *env, char *str);
int	ft_builtin_write_exit(t_env *env, char *input);
void	free_child(t_process_data *data, t_env *env);
int	ft_builtin_write_exit_process(t_process_data *data, t_env *env);
t_export		*init_export(t_export *export);
void			cleanup(pid_t *child_pids, int infile);
int				is_char_valid(char c);
int				print_error_export(char *s, int ok);
int				check_valid_identifier(char c);
int				check_valid_identifier_bis(char c);
void			update_env_pwd(t_env *env, char *new_pwd);
void			update_env_oldpwd(t_env *env);
void			print_expander_cmd(t_env *env, char *str, int i);
int				print_env_vars(t_env *env);
void			free_export(t_export *export);
int				error_input(t_env *env, t_command *new_cmd, char *input, int flag_ok);
void			ctrl_d_main(char *input, t_command *new_cmd, t_env *env_bis, int flag_ok);
int				check_valid_identifier_bis_bis(char c);
int				check_after_equal(char *str);
int				check_before_equal(char *s);
void			handle_signals_heredoc(t_process_data *data, char *line);
void			handle_heredoc_signals(t_process_data *data, char *line);
void			ft_close_all_fd(void);
void			ft_free_all(t_command *current, t_token *token);
void			ft_free_env(t_env *env);
void			print_commands_and_tokens(t_command *head);
void			ft_set_args_and_paths(t_command *cur, t_env *env);
char			**parse_input_quote(char *input);
int				builtins_verif(t_command *current, t_env	*env_bis);
char			**split_string_token(char *str, char **delimiters);
int				find_env_var(t_env *env, char *arg);
void			remove_env_var(t_env *env, int idx);
char			*extract_var_name(char *str);
int				export_expander(t_export *export, char *str, t_env *env);
int				count_pipe(char *input);
void			handle_quotes_master(char *str, int *i, bool *single_quote, bool *double_quote);
char			*epurstr(char *str);
void			verif_nb_args_exit(void);
int				verif_nb_pipe_exit(char **args);
int				validate_exit_status_process(char *exit_status_str);


/***********MAIN***********/

/***********BUILTINS***********/
void			update_env_pwd(t_env *env, char *new_pwd);
int				ft_builtin_cd(char **args, t_env *env);


int				ft_builtin_enter(char *input);
int				ft_is_all_space(char *input);

void			ft_builtin_env(t_env	*env);
void			copy_env(t_env *env, char **envp);

int				ft_builtin_export(char **args, t_env *env);

int				ft_builtin_echo(char **tab);
int				ft_builtin_pwd(int fd);

void			ft_builtin_unset(char **args, t_env *env);

int				check_pwd(t_command *current);
int				check_echo(t_command *current, t_env *env);
int				check_cd(t_command *current);

int				check_env(t_command *current, t_env *env);
int				check_unset(t_command *current);
int				check_export(t_command *current, t_env *env);

void			execve_builtins_unset_export(t_command *cmd, t_env *env_bis);
void			execve_builtin_cd(t_command *cmd, t_env *env_bis);

/***********EXECUTION***********/
t_command		*create_new_cmd(char *command_str, t_env *env);
t_command		*append_new_cmd(t_command **head, t_command *new_cmd);
t_command		*get_command(char *input, t_env *env);

void			handle_arg_value(t_arg_handler *arg_handler);
void			handle_all_process(t_process_data *data, t_env *env);

int				here_doc_ray(t_process_data *data, t_env *env);

void			init_execve(t_command *cur, pid_t **childs_pids);

/***********INIT_AND_PARSING***********/
// char			*ft_allocate_and_copy(t_arg_handler *arg_handler);
char *ft_allocate_and_copy(t_parser *parser);
void process_input_echo(t_parser *parser);


int				check_valid_identifier_export(char *str);

int				count_args_single_quotes(char *input);
int				count_args_double_quotes_args(char *input);

t_token			*handle_cmd_token(t_tokenizer *tz, t_env *env);
t_token			*handle_redir_tokens(t_tokenizer *tz);
t_token			*handle_arg_token(t_tokenizer *tz);
t_token			*handle_single_quote_token(t_tokenizer *tz);
t_token			*handle_double_quote_token(t_tokenizer *tz);

char			*create_str_from_arg(char *arg);
int				update_var_env(t_env *env, char *arg);
int				add_var_env(t_env *env, int i, char *str);

int				verif_nb_quote(char *input);
bool			contains_single_quote(char *str);
bool			contains_double_quote(char *str);

char			*env_var_exists(t_env *env, char *var);
char			*get_env_value(t_env *env, char *str);
void			append_env_value_to_arg(char *value, char *arg, int *arg_idx);

char			*handle_quotes_export(char *str);

t_token			*new_token(t_e_token_type e_type, char *split_value);
t_token			*tokenize_input(char *input, t_env *env);
char			*ft_check_paths_token(t_env *env, char *args);
char			*ft_check_paths(t_env *env, char *args);
void			execve_fd(t_command *current, t_env *env);

int				ft_builtin_enter(char *input);
int				ft_is_all_space(char *input);

char			**parse_arg_export(char *input);

char			**parse_input_quote_echo(t_env *env, char *input);
void			skip_spaces_echo(char *input, int *i);
// void			handle_quotes_echo(t_arg_handler *arg_handler);
void	handle_quotes_echo(char *str, int *i, bool *double_quote, bool *single_quote);

int				calculate_new_len(char *input);
char			*copy_with_spaces(char *input, char *new_input);
char			*add_spaces_around_redir(char *input);

/***********REDIRECTIONS***********/
char			*epur_filename(t_token *token_head);

char			*epur_filename_heredoc(t_token *token_head);

int				is_redir_at_beginning(char *input, int i);

int				redirect_file_in(t_process_data *data, t_env *env, t_token *token);
int				redirect_file_out(t_process_data *data, t_env *env, t_token *token);
int				redirect_append_file_out(t_process_data *data, t_env *env, t_token *token);
int				is_redir_at_beginning(char *input, int i);

void			redirect_append_file_out_open_fd(t_process_data *data, t_env *env, t_token *token);

void			heredoc_open_fd(t_process_data *data,t_command *command, t_token **token);
void			redirect_file_in_open_fd(t_process_data *data, t_env *env, t_token *token);
int				redirect_heredoc(t_command *current, t_token *token);
void			redirect_file_out_open_fd(t_process_data *data, t_env *env, t_token *token);

int				open_fd(t_process_data *data, t_env *env, t_command *command);

int				write_in_fd(int fd, char *delimiter, t_command *current);

/***********SIGNALS***********/
void			ctrl_c_main(int sig);
// void			handle_signals_heredoc(void);
int				ctrl_d_heredoc(char *input, int i, char *delimiter);
void			ctrl_c_heredoc(int signal);


/***********UTILS***********/
int				is_redirection(char c);
void			ft_skip_redirection_and_file(char *input, int *i);

int				check_valid_caractere_filename(char c);
int				check_syntax_errors(char *input);
int				pipe_syntax_errors(char *input);

void			ft_print_error(char *str);
void			exit_with_error(char *message, pid_t *child_pids);
void			print_error_cd(t_env *env, int i);

void			print_no_file_or_directory(t_env *env, char *args);
void			ft_print_error(char *str);
void			print_error_cd(t_env *env, int i);

char			*create_heredoc(void);

int				count_args(char *input, int i);
int				count_arg_length(char *input, int i);

void			ft_free_tab(char **tab);
void			ft_free_token(t_command *current);
void			ft_free_current(t_command *current);


void			free_tokens(char **tokens, int num);
char			**free_token_split(char **tokens);

int				ft_strchr_slash(char *str, char c);
int				ft_strncmp_minishell(char *s1, char *s2, int n);
int				ft_strcmp_minishell(char *s1, char *s2);
char			*ft_strjoin_minishell(char *s1, char *s2);

char			**split_command_on_pipe(char *input);

int				is_delimiter(char *str, char **delimiters, int *delim_len);
int				count_split_tokens_str(char *str, char **delimiters);
char			**allocate_tokens(int token_count);
char			*allocate_and_cpy(char *start, int size);

void			count_and_set_pipes(char *input, t_command *command);
void			ft_close_fd(void);
int				is_empty_or_space(char *str);
int				ft_isspace(int c);

int				calculate_size_of_argument(char *input);
void			initialize_bools(t_arg_handler *arg_handler);

char			**split_string(const char *str, char delimiter);

void			copy_env_in_return(t_export *export, t_env *env);
void			begin_var(t_export *export, char *str);
char			*re_init_var_str(t_export *export, char *str);

#endif