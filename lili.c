Pourquoi lorsque le programme passe dans la fonction "execve" les données associées a ma structure t_command dans "heredoc" ne s'affiche plus 



typedef struct s_command
{
	int					nb_pipes;
	char				*command;	// e.g. "cat test.txt"
	char				**command_arg;	// e.g. "cat"
	char				*command_path;	// e.g. /usr/bin/cat/
	int					fd[2];
	int					fd_out;
	int					fd_in;
	// int					nb_pipes;
	char				*file_name;
	char				*heredoc;
	struct s_token		*token_head;
	struct s_quote		*quote_head;
	struct s_command	*next;
} t_command;

l'execution : 


minishell$> cat << EOF
CURRENT heredoc DEBUT = (null)
WHILE execve_fd = (null)
> test
> end
> EOF
AAAAAAAAAAAAAAAAAAAAH = eazxrbuhol
START execve_process = eazxrbuhol
execve execve_process = eazxrbuhol
test
end

TEMP heredoc MIDDLE = (null)
CURRENT heredoc MIDDLE = (null)
heredoc = (null)


mon code :

void	execve_fd(t_command *current, t_env *env)
{
	t_process_data	data;
	t_command *tmp = current; // test

	printf("CURRENT heredoc DEBUT = %s\n", current->heredoc);

	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	init_execve(current, &(data.child_pids));
	while (data.current)
	{
		if (pipe(data.current->fd) == -1)
			exit_with_error("pipe", data.child_pids);
		data.pid = fork();
		data.child_pids[data.index++] = data.pid;
		data.current->fd_in = data.current->fd[0];
		data.current->fd_out = data.current->fd[1];
		handle_all_process(&data, env); // ici heredoc
		printf("WHILE execve_fd = %s\n", data.current->heredoc);
		data.current = data.current->next;
	}

	wait_for_children(data.command, data.child_pids);

	printf("\nTEMP heredoc MIDDLE = %s\n", tmp->heredoc);
	printf("CURRENT heredoc MIDDLE = %s\n", current->heredoc);

	while (tmp)
	{
		printf("heredoc = %s\n", tmp->heredoc);
		unlink(tmp->heredoc);
		tmp->heredoc = NULL; // Set the pointer to NULL after freeing to avoid double free errors
		tmp = tmp->next;
	}

	
	cleanup(data.child_pids, data.infile);
}

int	execve_process(t_command *cur, t_env *env)
{
	printf("START execve_process = %s\n", cur->heredoc);
	ft_set_args_and_paths(cur, env);
	if (env->flag_error || is_builtin(cur) == 2)
		exit(0);
	if ((cur->command_path == NULL) && is_builtin(cur) == 0)
	{
		write(2, "minishell: ", 11);
		write(2, cur->command_arg[0], ft_strlen(cur->command_arg[0]));
		write(2, " :command not found", 19);
		write(2, "\n", 1);
		ft_free_tab(cur->command_arg);
		ft_free_current(cur);
		return (127);
	}
	else if ((cur->command_path) && printf("execve execve_process = %s\n", cur->heredoc)
		&& (execve(cur->command_path, cur->command_arg, env->cpy_env) == -1))
	{
		perror("Error");
		return (-1);
	}
	return (0);
}
