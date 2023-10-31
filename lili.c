
Can you help me to fix my code ? 

In our heredoc if we have a command with a pipe, such as "cat << EOF | wc" we never manage to execute "wc" because it can't read what's in the heredoc, the probleme is that it wants to do so at the same time as we write to it. 

We put the opening of the heredoc in the parent process, we wanted to make a heredoc pid for the wait, and wait for the heredoc process to finish, then launch the child processes with the wc command, but it doesn't work at all ...

minishell$> cat << EOF | wc
> hi
> EOF
      0       0       0
minishell$> 

I need to use ctrl D to stop the process of the heredoc because "wc" is trying to read the heredoc but because I just free, wc can't count the number inside the heredoc. 

look at my code, do you want to see more ? is it possible to fix the problem ? If so, how can I do ? : 

void	ft_builtin_ctrl_c(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	global_ctrl_c_pressed = 1;
	rl_on_new_line(); // Informe readline que nous sommes sur une nouvelle ligne
	rl_replace_line("", 0); // Efface la ligne actuelle
	rl_redisplay(); // Réaffiche le prompt
}

void	sighandler_heredoc(int sig)
{
	int fd;

	(void)sig;
	fd = open ("/dev/null", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	printf("\n");
	global_ctrl_c_pressed = 130;
	return ;
}

void handle_signals_heredoc()
{
	global_ctrl_c_pressed = 0;
	signal(SIGINT, sighandler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

int	ctrl_d_heredoc(char *input, int i, char *delimiter)
{
	if (!input)
	{
		write(1, "minishell: warning: here-document at line ", 43);
		ft_putnbr_fd(i, STDOUT_FILENO);
		write(1," delimited by end-of-file (wanted \'", 36);
		ft_putstr_fd(delimiter, STDOUT_FILENO);
		write(1, "')\n", 4);
		return (45);
	}
	return(0);
}

int	write_in_fd(int fd, char *delimiter, t_command *current)
{
	char	*line;
	int		i;

	i = 0;
	handle_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		if (global_ctrl_c_pressed == 130)
		{
			clean_heredoc_files(current);
			free(line);
			exit(130);
		}
		if (ctrl_d_heredoc(line, i, delimiter) == 45)
		{
			return (45);
		}
		if (ft_strcmp_minishell(line, delimiter) == 0)
			break ;
		
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		i++;
		free(line);
	}
	if (line)
		free(line);
	return (0);
}

void	add_to_heredocs_list(t_command *current, char *heredoc_name)
{
	int	i;
	
	i = 0;
	if (!current->heredocs)
	{
		current->heredocs = malloc(sizeof(char*) * 2);
		if (!current->heredocs)
			return ;
		current->heredocs[0] = ft_strdup(heredoc_name);
		current->heredocs[1] = NULL;
		return ;
	}
	while (current->heredocs[i])
		i++;
	current->heredocs = ft_realloc(current->heredocs, sizeof(char*) * (i + 1), sizeof(char*) * (i + 2));
	current->heredocs[i] = ft_strdup(heredoc_name);
	current->heredocs[i + 1] = NULL;
}

t_token	*handle_multiple_heredocs(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	while (token && ft_strcmp_minishell(token->split_value, "<<") == 0)
	{
		delimiter = extract_filename_heredoc(current->command);
		if (current->heredoc)
			free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
		add_to_heredocs_list(current, current->heredoc);
		write_in_fd(fd, delimiter, current);
		fd = open(current->heredoc, O_RDONLY);
		current->fd_in = fd;
		if (current->fd_in == -1)
		{
			perror("minishell: EOF");
			exit(-1);
		}
		token = token->next->next;
	}
	return(token);
}

int	redirect_heredoc(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	fd = -1;
	delimiter = token->next->split_value;
	if (fd == -1)
	{
		if (current->heredoc)
			free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
	}
	write_in_fd(fd, delimiter, current);
	fd = open(current->heredoc, O_RDONLY);
	current->fd_in = fd;
	if (current->fd_in == -1)
	{
		perror("minishell: EOF");
		exit(-1);
	}
	return (0);
}

int	open_fd(t_command *command)
{
	t_token	*token;
	t_token	*token_head;

	token = command->token_head;
	token_head = command->token_head;
	set_last_redirection_flag(command, token_head);
	while (token)
	{
		if ((token->type == TYPE_HEREDOC) && (command->nb_pipes == 0))
			heredoc_open_fd(command, &token);
		else
			token = token->next;
	}
	token = command->token_head;
	while (token)
	{
		redirect_file_in_open_fd(command, token, token_head);
		redirect_file_out_open_fd(command, token, token_head);
		redirect_append_file_out_open_fd(command, token, token_head);
		token = token->next;
	}
	return (0);
}

void	heredoc_open_fd(t_command *command, t_token **token)
{
	if (*token && (*token)->type == TYPE_HEREDOC)
	{
		*token = handle_multiple_heredocs(command, *token);
		if (command->fd_in != -1)
		{
			dup2(command->fd_in, 0);
			close(command->fd_in);
		}
	}
}

pid_t heredoc_open_fd_pipe(t_command *command, t_token **token)
{
	pid_t pid = -1;

	if (*token && (*token)->type == TYPE_HEREDOC)
	{
		pid = fork();
		if (pid == 0) 
		{
			*token = handle_multiple_heredocs(command, *token);

			if (command->fd_in != -1)
			{
				dup2(command->fd_in, 0);
				close(command->fd_in);
			}
			exit(0);
		}
		else if (pid < 0) 
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
	}
	return (pid); 
}

void	init_execve(t_command *cur, pid_t **childs_pids)
{
	int	nb_child;

	nb_child = 0;
	while (cur)
	{
		(nb_child)++;
		cur = cur->next;
	}
	*childs_pids = malloc(nb_child * sizeof(pid_t));
	if (!*childs_pids)
		exit(1);
}

void	ft_set_args_and_paths(t_command *cur, t_env *env)
{
	cur->command_arg = NULL;
	cur->command_path = NULL;
	cur->command_arg = parse_input_quote(cur->command);
	cur->command_path = ft_check_paths(env, cur->command_arg[0]);
}

void	handle_child_process(t_process_data *data, t_env *env)
{
	close(data->current->fd_in); // a priori close le pipe du heredoc et ne permet plus de le lire
	dup2(data->infile, 0);
	if (data->current->next)
		dup2(data->current->fd_out, 1);
	close(data->current->fd_out);
	ft_close_fd();
	if (data->current->fd_in != -1)
	{
		dup2(data->current->fd_in, 0);
		close(data->current->fd_in);
	}
	if (data->current->fd_out != -1)
	{
		dup2(data->current->fd_out, 1);
		close(data->current->fd_out);
	}
	open_fd(data->current);
	if (builtins_verif(data->current, env) == 1)
		exit(0);
	if (execve_process(data->current, env) == 127)
		exit(127);
}

void	handle_parent_process(t_process_data *data)
{
	close(data->current->fd_out);
	if (data->infile != 0)
		close(data->infile);
	data->infile = data->current->fd_in;
}

void	handle_all_process(t_process_data *data, t_env *env)
{
	if (data->pid == 0)
	{
		handle_child_process(data, env);
		clean_heredoc_files(data->current);
	}
	else if (data->pid > 0)
		handle_parent_process(data);
	else
		exit_with_error("fork", data->child_pids);
}

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;
	int	status; // AJOUT
	i = 0;
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		// ++i;
		waitpid(child_pids[i], &status, 0); // AJOUT
		i++;
	}
	signal(SIGINT, ft_builtin_ctrl_c);
}

void execve_fd(t_command *current, t_env *env)
{
	t_process_data data;
	t_token *token;
	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	token = current->token_head;
	init_execve(current, &(data.child_pids));

	int flag = 0;
	if (data.current->nb_pipes != 0)
	{
		while(data.current && flag == 0)
		{
			while(token && flag == 0)
			{
				if (token->type == TYPE_HEREDOC)
				{
					
					pid_t heredoc_pid = heredoc_open_fd_pipe(current, &token);
					waitpid(heredoc_pid, NULL, 0);
					flag = 1;
					break;
				}
				token = token->next;
			}
			if (flag == 1)
				break;
			
			data.current = data.current->next;
		}
	}
	data.current = current;
	while (data.current)
	{
		if (pipe(data.current->fd) == -1)
			exit_with_error("pipe", data.child_pids);
		data.pid = fork();
		data.child_pids[data.index++] = data.pid;
		data.current->fd_in = data.current->fd[0];
		data.current->fd_out = data.current->fd[1];
		handle_all_process(&data, env);
		data.current = data.current->next;
	}
	waitpid(data.pid, NULL, 0); // omg
	cleanup(data.child_pids, data.infile);
}

int	is_builtin(t_command *cur)
{
	if (ft_strcmp_minishell(cur->command, "exit") == 0)
		return (2);
	if ((ft_strcmp_minishell(cur->command_arg[0], "cd") == 0)
		|| (ft_strncmp(cur->command_arg[0], "cd ", 3) == 0))
		return (1);
	if ((ft_strcmp_minishell(cur->command, "echo") == 0)
		|| (ft_strncmp(cur->command, "echo ", 5) == 0))
		return (1);
	if (ft_strcmp_minishell(cur->command, "env") == 0)
		return (1);
	if ((ft_strcmp_minishell(cur->command, "export") == 0)
		|| (ft_strncmp(cur->command, "export ", 7) == 0))
		return (1);
	if (ft_strcmp_minishell(cur->command, "pwd") == 0)
		return (1);
	if ((ft_strcmp_minishell(cur->command, "unset") == 0)
		|| (ft_strncmp(cur->command, "unset ", 6) == 0))
		return (1);
	return (0);
}

int	execve_process(t_command *cur, t_env *env)
{
	clean_heredoc_files(cur);
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
	else if ((cur->command_path)
		&& (execve(cur->command_path, cur->command_arg, env->cpy_env) == -1))
	{
		perror("Error");
		return (-1);
	}
	return (0);
}
