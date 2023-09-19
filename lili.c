peux tu maider a resoudre le probleme de mes fds in qui sont tous a 0 je ne sais pas pourquoi ils ne changent pas avec lentrree des pipes comme ils devraient le faire


➜  min git:(main) ✗ ./minishell 
minishell$> cat celine.txt | rev | rev | rev 
Current fd_in: 0, fd_out: 4

dup2 fd_out: Bad file descriptor
Current fd_in: 0, fd_out: 5

dup2 fd_out: Bad file descriptor
Current fd_in: 0, fd_out: 6

dup2 fd_out: Bad file descriptor
Current fd_in: 0, fd_out: 1

rev: stdin: 0: Bad file descriptor
minishell$> 

int child_process(t_command *current, char **envp)
{
	printf("Current fd_in: %d, fd_out: %d\n\n", current->fd_in, current->fd_out);
	if (current->fd_in != -1)
	{
		if (dup2(current->fd_in, 0) == -1)
		{
			perror("dup2 fd_in");
			exit(EXIT_FAILURE);
		}
		close(current->fd_in);
	}

	if (current->fd_out != -1)
	{
		if (dup2(current->fd_out, 1) == -1)
		{
			perror("dup2 fd_out");
			exit(EXIT_FAILURE);
		}
		close(current->fd_out);
	}
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

int redirect_file_out(t_command **cmd, t_token *cur)
{
    char *filename;

    if ((*cmd)->fd_out != 1)
        close((*cmd)->fd_out);

    filename = cur->next->split_value;
    (*cmd)->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if ((*cmd)->fd_out == -1)
    {
        perror(filename);
        return (-1);
    }
    return (0);
}

int    open_fd(t_command **cmdl)
{
	t_token    *cur;

	cur = (*cmdl)->token_head;

	// Initialisation des valeurs par défaut
	(*cmdl)->fd_in = 0; 
	(*cmdl)->fd_out = 1;
	
	while (cur)
	{
		if (cur->type == TYPE_REDIR_OUT)
		{
			if (redirect_file_out(cmdl, cur) == -1)
				return (-1);
		}
		// Ajoutez d'autres conditions pour gérer d'autres types de redirections
		// ...
		cur = cur->next;
	}
	return (0);
}

void execve_fd(t_command *current, char **envp)
{
	t_command    *command;
	pid_t        pid;
	pid_t        *child_pids;
	int            num_children;
	int            index;
	int            i;

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
	while (current)
	{
		open_fd(&current);
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
			// Si une commande suivante existe, alors nous avons un pipe, utilisez fd_out de l'enfant pour écrire dans le pipe
			if (current->next) 
				current->fd_out = current->fd[1];
			close(current->fd[0]);
			close_fd();
			child_process(current, envp);
		}
		else if (pid > 0) // Parent
		{
			close(current->fd[1]);
			if(current->next)  // Si une commande suivante existe, alors fd_in de cette commande doit lire depuis le pipe
				current->next->fd_in = current->fd[0];
			if(current->fd_out != 1)
				close(current->fd_out);
		}
		else
		{
			perror("fork");
			free(child_pids);
			exit(1);
		}
		current = current->next;
	}
	// Gestion des signaux et attente des fils
	signal(SIGINT, SIG_IGN);
	for (i = 0; i < num_children; i++)
		waitpid(child_pids[i], NULL, 0);
	signal(SIGINT, ft_signal_ctrl_C);
	free(child_pids);
}