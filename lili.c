jai un petit probleme dans la gestiondouverture et de fermeture de mes fds peux tu maider


int	child_process(t_command *current, char **envp)
{
	dup2(current->fd_in, 0);
	dup2(current->fd_out, 1);
	
	close(current->fd_in);
	close(current->fd_out);
	
	ft_set_args_and_paths(current, envp);
	if (current->command_path == NULL)
	{
		write(2, "minishell: command not found: ", 31);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, "\n", 1);
		ft_free_tab(current->command_arg);
		ft_free_current(current);
		return (127);
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1)
	{
		perror("Error");
		exit(-1);
	}
	return (0);
}

int	redirect_file_out(t_command **cmd, t_token *cur)
{
	char	*filename;

	if ((*cmd)->fd_out != 1)
		close((*cmd)->fd_out);
	filename = cur->next->split_value;
	// printf("%s\n", filename); //printf
	(*cmd)->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	// printf("(*cmd)->fd_out : %d\n", (*cmd)->fd_out); //printf
	if ((*cmd)->fd_out == -1)
	{
		perror(filename);
		return (-1);
	}
	return (0);
}

int	open_fd(t_command **cmdl)
{
	t_token	*cur;

	cur = (*cmdl)->token_head;
	(*cmdl)->fd_in = -1;
	(*cmdl)->fd_out = -1;
	
	while (cur)
	{
		if (cur->type == TYPE_REDIR_OUT)
		{
			// if (redirect_file_out(cmdl, cur, cur->type) == -1)
			if (redirect_file_out(cmdl, cur) == -1)
				return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

void execve_fd(t_command *current, char **envp)
{
	t_command	*command;
	pid_t		pid;
	pid_t		*child_pids;
	int			num_children;
	// int			infile;
	int			index;
	int			i;

	command = current;
	current->fd_in = 0;
	num_children = 0;
	index = 0;
	i = -1;
	while (current)
	{
		num_children++;
		current = current->next;
	}
	current = command;
	child_pids = malloc(num_children * sizeof(pid_t));
	if (!child_pids)
		exit(1);
	open_fd(&current);
	while (current)
	{
		if (pipe(current->fd) == -1)
		{
			perror("pipe");
			free(child_pids);
			exit(1);
		}
		pid = fork();
		child_pids[index++] = pid;
		if (pid == 0) // Child
		{
			if (current->next) // a deplacer dans le child ? 
				current->fd_out = current->fd[1];
			else
				current->fd_out = 1;
			
			close(current->fd[0]); // a deplacer dans le child ? 
			close(current->fd[1]); // a deplacer dans le child ? 
			
			close_fd();
			if(child_process(current, envp) == 127)
			{
				free(child_pids);
				exit(127);
			}
		}
		else if (pid > 0) // Parent
		{
			close(current->fd[1]);
			// if(current->fd_out)
			if(current->fd_out != 1)
				close(current->fd_out);
			if (current->fd_in != 0)
				close(current->fd_in);
			current->fd_in = current->fd[0];
		}
		else
		{
			perror("fork");
			free(child_pids);
			exit(1);
		}
		current = current->next;
	}
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		++i;
		waitpid(child_pids[i], NULL, 0);
	}
	signal(SIGINT, ft_signal_ctrl_C);
	free(child_pids);
	// if (current->fd_in != 0)
		// close(current->fd_in);
}

