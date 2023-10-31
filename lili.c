void	handle_child_process(t_process_data *data, t_env *env)
{
    close(data->current->fd_in);
    
    if (data->infile != 0)
    {
        dup2(data->infile, STDIN_FILENO);
    }
    else
    {
        dup2(data->current->fd_in, STDIN_FILENO);
    }
    
    if (data->current->next)
    {
        dup2(data->current->fd_out, STDOUT_FILENO);
    }
    
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
		handle_child_process(data, env);
	else if (data->pid > 0)
		handle_parent_process(data);
	else
		exit_with_error("fork", data->child_pids);
}

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;

	i = -1;
	printf("3bis - sig\n");
	signal(SIGINT, SIG_IGN);
	printf("3bis - sig after \n");
	while (i < command->nb_pipes)
	{
		++i;
		printf("i = %d\n", i);
		waitpid(child_pids[i], NULL, 0);
	}
	printf("3bis - olala sig\n");
	signal(SIGINT, ft_builtin_ctrl_c);
	printf("3bis - FIN sig\n");
}

pid_t heredoc_open_fd_pipe(t_command *command, t_token **token, int heredoc_fd[2])
{
	pid_t pid = -1;

	if (*token && (*token)->type == TYPE_HEREDOC)
	{
		if (pipe(heredoc_fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		pid = fork();
		if (pid == 0) 
		{
			close(heredoc_fd[0]);
			dup2(heredoc_fd[1], STDOUT_FILENO);
			close(heredoc_fd[1]);
			
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
			close(heredoc_fd[0]);
			close(heredoc_fd[1]);
			perror("fork");
			exit(EXIT_FAILURE);
		}
		close(heredoc_fd[1]);  // Nous n'écrivons pas dans ce pipe dans le processus parent
	}
	return pid; 
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
					
					pid_t heredoc_pid = heredoc_open_fd_pipe(current, &token, data.heredoc_fd);
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
	printf("1 - j'ai fini de wait le pid de heredoc\n");
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
	printf("2 - je vais wait le pid\n");
	waitpid(data.pid, NULL, 0); // omg
	printf("3 - j'ai wait le pid\n");
	cleanup(data.child_pids, data.infile);
}