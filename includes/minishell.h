/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/08 13:11:06 by abonnefo         ###   ########.fr       */
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
# define SIZE 1000
# define SIZE_VAR 5000
# define RESET "\033[0m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"

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

typedef struct s_parser
{
	char	**args;
	int		i;
	int		idx;
	bool	in_quote;
}	t_parser;

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
	TYPE_HEREDOC,
	TYPE_F_OUT,
	TYPE_F_IN,
	TYPE_EOF,
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
	char	*str;
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
	char		*delimiter;
	int			execute_builtin_later;
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
	char				*heredoc;
	char				**heredocs;
	int					flag_chevron;
	bool				last_redir_is_heredoc;
	int					exit_status;
	int					flag;
	struct s_token		*token_head;
	struct s_quote		*quote_head;
	struct s_command	*next;
}	t_command;

extern int					g_exit_status;

/*******************************   BUILTINS   *******************************/
int				error_input(t_env *env, char *input);
int				check_g_expander(t_command *current);
int				ret_place_var(t_command *current, t_env *env, char *str);
int				check_expand(t_command *current, t_env *env);
int				builtins_verif(t_command *current, t_env *env);

char			*allocate_var_name(void);
char			*extract_var_name_cd(t_env *env, char *ret, int *i, int *m);
char			*check_expander(t_env *env);
int				ft_builtin_cd(char **args, t_env *env);

int				ft_builtin_echo(char **tab);

void			copy_env(t_env *env, char **envp);
int				check_equal_env(char *str);
void			ft_builtin_env(t_env *env);

char			*extract_var_name(char *str);
int				ft_builtin_export(char **args, t_env *env);

int				ft_builtin_pwd(int fd);

void			remove_env_var(t_env *env, int idx);
int				find_env_var(t_env *env, char *arg);
void			ft_builtin_unset(char **args, t_env *env);

int				validate_exit_status_process(char *exit_status_str);
int				ft_builtin_write_exit_process(t_process_data *data, t_env *env);

int				ft_builtin_write_exit(t_env *env, char *input);

int				check_pwd(t_command *current);
int				check_echo(t_command *current, t_env *env);
int				check_cd(t_command *current);

int				check_env(t_command *current, t_env *env);
int				check_unset(t_command *current);
int				check_export(t_command *current, t_env *env);

void			execve_builtins_unset_export(t_command *cmd, t_env *env_bis);
int				count_cd_args(char *command_arg);
void			execve_builtin_cd(t_command *cmd, t_env *env_bis);

char			*check_none_var(char *str);
int				apply_var_name(t_export *export, char **str, t_env *env,
					char *var_name);
int				expand_variable(t_export *export, char **str, t_env *env);
int				export_expander(t_export *export, char *str, t_env *env);

/******************************   EXECUTION   *******************************/
int				execve_process(t_process_data *data, t_env *env);

void			handle_arg_value(t_arg_handler *arg_handler);

void			free_child(t_process_data *data, t_env *env);
void			handle_all_process(t_process_data *data, t_env *env);

char			**parse_input_quote(char *input);

int				here_doc_ray(t_process_data *data, t_env *env);

void			execve_fd(t_command *current, t_env *env);

/***************************   INIT_AND_PARSING   ***************************/
char			*ft_allocate_and_copy(t_arg_handler *arg_handler);

int				check_before_equal(char *s);
int				check_after_equal(char *str);

char			*check_expr(char *str);

void			check_invalid_var(t_env *env, char *str);

int				is_char_valid(char c);
int				print_error_export(char *s, int ok);
int				check_valid_identifier(char c);
int				check_valid_identifier_bis(char c);
int				check_valid_identifier_bis_bis(char c);

int				count_args_single_quotes(char *input);
int				count_args_double_quotes_args(char *input);

t_command		*create_new_cmd(char *cmd_str, t_env *env);
t_command		*append_new_cmd(t_command **head, t_command *new_cmd);
t_command		*get_command(char *input, t_env *env);

char			*handle_quotes_export(char *str);

t_token			*handle_cmd_token(t_tokenizer *tz, t_env *env);
t_token			*handle_redir_tokens(t_tokenizer *tz);
t_token			*handle_arg_token(t_tokenizer *tz);
t_token			*handle_single_quote_token(t_tokenizer *tz);
t_token			*handle_double_quote_token(t_tokenizer *tz);

char			*create_str_from_arg(char *arg);
int				update_var_env(t_env *env, char *arg);
int				add_var_env(t_env *env, int i, char *str);

void			init_execve(t_command *cur, pid_t **childs_pids);
void			ft_set_args_and_paths(t_process_data *data, t_env *env);
int				is_builtin(t_command *cur);
int				check_args_and_env(int ac, char **envp);
t_env			*initialize_env(char **envp);

char			*get_env_value(t_env *env, char *str);
void			append_env_value_to_arg(char *value, char *arg, int *arg_idx);
t_process_data	*return_data(t_process_data *data);
t_env			*return_env(t_env *env);

t_export		*init_export(t_export *export);
char			*re_init_var_str(t_export *export, char *str);
void			begin_var(t_export *export, char *str);
void			copy_env_in_return(t_export *export, t_env *env);

int				verif_nb_single_quote(char *str);
int				verif_nb_double_quote(char *str);
bool			contains_single_quote(char *str);
bool			contains_double_quote(char *str);

t_token			*new_token(t_e_token_type e_type, char *split_value);
void			add_token_to_list(t_token **head, t_token **tail,
					t_token *new_tok);
void			init_tokenizer(t_tokenizer *tz, char *input);
t_token			*create_token(t_tokenizer *tz, t_env *env);
t_token			*tokenize_input(char *input, t_env *env);

char			*add_sp_ard_redir(char *input, t_env *env);

void			write_exit_simple(t_env *env, char *str);
int				ft_builtin_enter(char *input);
int				ft_is_all_space(char *input);
void			ctrl_d_main(char *input, t_command *new_cmd, t_env *env_bis,
					int flag_ok);

char			**parse_arg_export(char *input);

void			skip_spaces_echo(char *input, int *i);
char			**parse_input_quote_echo(t_env *env, char *input);

char			*ft_check_paths_token(t_env *env, char *args);

char			*ft_check_paths(t_env *env, char *args);

void			update_env_pwd(t_env *env, char *new_pwd);
void			update_env_oldpwd(t_env *env);

int				verif_nb_quote(char *input);

/*****************************   REDIRECTIONS   *****************************/
char			*epur_filename(t_token *token_head);

char			*epur_filename_heredoc(t_token *token_head);

int				is_redir_at_beginning(char *input, int i);

char			*verif_file_name(t_process_data *data, t_env *env,
					t_token *token);
int				redirect_file_out(t_process_data *data, t_env *env,
					t_token *token);
int				redirect_file_in(t_process_data *data, t_env *env,
					t_token *token);
int				redirect_append_file_out(t_process_data *data,
					t_env *env, t_token *token);

void			heredoc_open_fd(t_process_data *data, t_command *command,
					t_token **token);
void			redirect_file_in_open_fd(t_process_data *data, t_env *env,
					t_token *token);
void			redirect_file_out_open_fd(t_process_data *data, t_env *env,
					t_token *token);
void			redirect_append_file_out_open_fd(t_process_data *data,
					t_env *env, t_token *token);

int				open_fd(t_process_data *data, t_env *env, t_command *command);

/*******************************   SIGNALS   ********************************/
void			ctrl_c_main(int signal);
int				ctrl_d_heredoc(char *input, int i, char *delimiter);
void			ctrl_c_manage(int signal);
void			ctrl_c_heredoc(int signal);
void			verif_ctrl_c(t_process_data *data, t_env *env);

/********************************   UTILS   *********************************/
int				is_redirection(char c);
void			ft_skip_redirection_and_file(char *input, int *i);
int				check_bad_redir(char *input);

int				check_valid_caractere_filename(char c);

int				pipe_syntax_errors(char *input);

int				count_args(char *input, int i);
int				count_arg_length(char *input, int i);

void			exit_with_error(char *message, pid_t *child_pids);
int				is_dir_error(char *command);

void			exit_access_exec(t_process_data *data, t_env *env,
					char *command);
void			exit_access_not_found(t_process_data *data, t_env *env,
					char *command);
int				is_dir_error(char *command);
int				check_dots_commands(t_process_data *data, t_env *env,
					char *command);
int				verif_access(t_process_data *data, t_env *env, char *command);

void			free_export_str(t_export *export);
void			free_export_basic(t_export *export);

void			ft_free_tab(char **tab);
void			free_tokens(char **tokens, int num);
char			**free_token_split(char **tokens);
void			ft_free_all(t_command *current, t_token *token);
void			cleanup(pid_t *child_pids, int infile);

void			ft_free_env(t_env *env);
void			free_export_basic(t_export *export);
void			free_export_str(t_export *export);
void			ft_free_herdocs(t_command *current);
void			ft_free_token(t_command *current);
void			ft_free_current(t_command *current);

void			handle_quotes_master(char *str, int *i, bool *single_quote,
					bool *double_quote);
char			*epurstr(char *str);

int				ft_strchr_slash(char *str, char c);
int				ft_strncmp_minishell(char *s1, char *s2, int n);
int				ft_strcmp_minishell(char *s1, char *s2);
char			*ft_strjoin_minishell(char *s1, char *s2);

void			print_no_file_or_directory(t_env *env, char *args);
void			ft_print_error(char *str);
void			print_error_cd(t_env *env, int i);

void			print_export(char *str, int fd);
int				print_env_vars(t_env *env);
void			print_expander_cmd(t_env *env, char *str, int i);
void			skip_var_name(char *str, int *i);

char			**split_command_on_pipe(char *input);

char			**split_string_token(char *str, char **delimiters);
int				count_split_tokens_str(char *str, char **delimiters);
char			**allocate_tokens(int token_count);
char			*allocate_and_cpy(char *start, int size);

int				is_delimiter(char *str, char **delimiters, int *delim_len);

char			**split_string(const char *str, char delimiter);

int				count_pipe(char *input);
void			verif_nb_args_exit(void);
int				verif_nb_pipe_exit(char **args);

char			*ft_get_env(const char *name, t_env *env);
int				pass_find_var_name(t_env *env, char *var_name, int *i, int j);

void			handle_quotes_echo(char *str, int *i, bool *double_quote,
					bool *single_quote);
int				calculate_size_of_argument(char *input);
void			initialize_bools(t_arg_handler *arg_handler);

void			count_and_set_pipes(char *input, t_command *current);
void			ft_close_fd(void);
void			ft_close_all_fd(void);
int				is_empty_or_space(char *str);
int				ft_isspace(int c);

#endif