peux tu me montrer pourquoi je ne narrive pas a renvoyer la sortie de la commande "cat celine.txt | wc > test.txt" dans le fichier test.txt.


void	close_fds()
{
	int	fd;

	fd = 3;
	while (fd < 100)
	{
		close(fd);
		fd++;
	}
}

int	redirect_file_out(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = token->next->split_value;
	// printf("%s\n", filename); //printf
	current->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	// dup2(current->fd_out, 1);
	// printf("current->fd_out : %d\n", current->fd_out); //printf
	if (current->fd_out == -1)
	{
		perror(filename);
		return (-1);
	}
	return (0);
}

int	open_fd(t_command *command)
{
	t_token	*token;

	token = command->token_head;
	while (token)
	{
		if (token->type == TYPE_REDIR_OUT)
		{
			if (redirect_file_out(command, token) == -1)
				return (-1);
		}
		token = token->next;
	}
	return (0);
}

void execve_fd(t_command *current, char **envp)
{
	t_command	*command;
	pid_t		pid;
	pid_t		*child_pids;
	int			infile;
	int			num_children;
	int			index;
	int			i;

	command = current;
	infile = 0;
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
		current->fd_in = current->fd[0]; // init apres la creation des pipes
		current->fd_out = current->fd[1];
		if (pid == 0) // Child
		{
			close(current->fd_in);
			dup2(infile, 0);
			if (current->next)
				dup2(current->fd_out, 1);
			close(current->fd_out);
			close_fd();
			if (current->fd_in != -1)
			{
				dup2(current->fd_in, 0);
				close(current->fd_in);
			}
			if (current->fd_out != -1)
			{
				dup2(current->fd_out, 1);
				close(current->fd_out);
			}
			if (open_fd(current) == 0)
			{
				dup2(current->fd_out, 1);
				close(current->fd_out);
			}
			ft_set_args_and_paths(current, envp);
			////////////////////////////
			// int j = 0;
			// while(current->command_arg[j])
			// {
			// 	printf("arguments %d : %s\n", j, current->command_arg[j]);
			// 	j++;
			// }
			// printf("%sfd_out: %d%s\n\n", MAGENTA, current->fd_out, RESET);
			///////////////////////////
			if (current->command_path == NULL)
			{
				write(2, "minishell: command not found: ", 31);
				write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
				write(2, "\n", 1);
				ft_free_tab(current->command_arg);
				ft_free_current(current);
				// return (127);
			}
			else if (execve(current->command_path, current->command_arg, envp) == -1)
			{
				perror("Error");
				exit(-1);
			}
		}
		else if (pid > 0) // Parent
		{
			close(current->fd_out);
			if (infile != 0)
				close(infile);
			infile = current->fd_in;
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
	if (infile != 0)
		close(infile);
}
