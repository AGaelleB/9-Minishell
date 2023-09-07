
typedef enum e_token_type {
	TYPE_CMD,
	TYPE_ARG,
	TYPE_SEPARATOR,
	TYPE_REDIR_OUT,		// ">"
	TYPE_REDIR_IN,		// "<"
	TYPE_REDIR_APPEND,	// ">>"
	TYPE_BUILTIN,
	TYPE_HEREDOC,
	TYPE_EOF
} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value;
	struct s_token		*next;
} t_token;

typedef struct s_command
{
	char				*command;			// e.g. "cat test.txt"
	char				**command_arg;		// e.g. "cat"
	char				*command_path;		// e.g. /usr/bin/cat/
	// char	separator;			// e.g. '|'
	int					input_fd;
	int					output_fd;
	struct s_token		*token;
	struct	s_command	*next;	// Pointer vers la prochaine commande
} t_command;

t_command	*get_command(char *input)
{
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;
	char		**command;
	int			i;

	head = NULL;
	current = NULL;
	command = split_string(input, '|');
	i = 0;
	while(command[i])
	{
		new_cmd = malloc(sizeof(t_command));
		new_cmd->command = ft_strdup(command[i]);
		if (!head)
		{
			head = new_cmd;
			current = head;
		}
		else
		{
			current->next = new_cmd;
			current = new_cmd;
		}
		i++;
	}
	return (current);
}

void	ft_set_args_and_paths(t_command *current, char **envp)
{
	current->command_arg = ft_split(current->command, ' ');
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
}

void	child_proces(t_command *current, int read_fd, int write_fd, char **envp)
{
	dup2(read_fd, 0);
	if (write_fd != 1) // Si ce n'est pas la sortie standard
		dup2(write_fd, 1);
	close(read_fd);
	close(write_fd);
	ft_set_args_and_paths(current, envp);
	if ((current->command_path != NULL)
		&& (execve(current->command_path, current->command_arg, envp) == -1))
	{
		perror("Error");
		//free
		exit(-1);
	}
}

void	exec_pipe(t_command *current, int read_fd, int write_fd, char **envp)
{
	int pid;
	
	pid = fork();
	if (pid == 0)
		child_proces(current, read_fd, write_fd, envp);
	waitpid(pid, NULL, 0);
}

void	multiple_pipe(t_command *current, char **envp, int infile, int outfile)
{
	int	fd[2];
	int	read_fd;
	int write_fd;


	read_fd = infile;
	while (current)
	{
		pipe(fd);
		if (current->next)
			write_fd = fd[1];
		else
			write_fd = outfile;
		exec_pipe(current, read_fd, write_fd, envp);
		close(fd[1]);
		read_fd = fd[0];
		current = current->next;
	}
	//free
}

t_token *new_token(t_e_token_type e_type, char *split_value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);

	token->type = e_type;
	token->split_value = ft_strdup(split_value);  // Remember to free this later!
	token->next = (NULL);

	return (token);
}

t_token *tokenize_input(char *input)
{
	char		**words;
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	int			i;

	words = split_string(input, ' ');  // Assuming a space delimiter
	head = NULL;
	curr = NULL;
	i = 0;
	while (words[i])
	{
		token = NULL;

		if (strcmp(words[i], "|") == 0)
			token = new_token(TYPE_SEPARATOR, words[i]);
		else if (strcmp(words[i], ">") == 0)
			token = new_token(TYPE_REDIR_OUT, words[i]);
		else if (strcmp(words[i], "<") == 0)
			token = new_token(TYPE_REDIR_IN, words[i]);
		else if (strcmp(words[i], ">>") == 0)
			token = new_token(TYPE_REDIR_APPEND, words[i]);
		else if (i == 0 ||
				(i > 0 && (strcmp(words[i - 1], "|") == 0 || 
							strcmp(words[i - 1], ">") == 0 || 
							strcmp(words[i - 1], "<") == 0 || 
							strcmp(words[i - 1], ">>") == 0)))
			token = new_token(TYPE_CMD, words[i]);
		else
			token = new_token(TYPE_ARG, words[i]);

		if (!head)
		{
			head = token;
			curr = head;
		}
		else
		{
			curr->next = token;
			curr = token;
		}
		i++;
	}
	return (head);
}

int redirect_file_in(t_command *cmd, t_token *token)
{
	if (token->type == TYPE_REDIR_IN)
	{
		cmd->input_fd = open(token->next->split_value, O_RDONLY);
		if (cmd->input_fd == -1)
		{
			perror("Error opening file for input redirection");
			return (-1);
		}
	}
	else if (token->type == TYPE_SEPARATOR)
	{
		// La gestion des pipes sera gérée dans la fonction `multiple_pipe`.
	}
	return (0);
}

int redirect_file_out(t_command *cmd, t_token *token)
{
	if (token->type == TYPE_REDIR_OUT)
	{
		cmd->output_fd = open(token->next->split_value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (cmd->output_fd == -1)
		{
			perror("Error opening file for output redirection");
			return (-1);
		}
	}
	else if (token->type == TYPE_REDIR_APPEND)
	{
		cmd->output_fd = open(token->next->split_value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (cmd->output_fd == -1)
		{
			perror("Error opening file for output appending");
			return (-1);
		}
	}
	else if (token->type == TYPE_SEPARATOR)
	{
		// La gestion des pipes sera gérée dans la fonction `multiple_pipe`.
	}
	return (0);
}

int open_fd(t_command *current, char **envp)
{
	t_token *token;

	while (current)
	{
		token = current->token;
		while (token)
		{
			if (token->type == TYPE_REDIR_IN)
			{
				redirect_file_in(current, token);
				token = token->next;  // pour passer le nom du fichier
			}
			else if (token->type == TYPE_REDIR_OUT || token->type == TYPE_REDIR_APPEND)
			{
				redirect_file_out(current, token);
				token = token->next;  // pour passer le nom du fichier
			}
			token = token->next;
		}
		multiple_pipe(current, envp, current->input_fd, current->output_fd);
		current = current->next;
	}
	return (0);
}


int	main(int ac, char **av, char **envp)
{
	// t_token		*token;
	t_command	*current;
	char		*input;

	while (1)
	{
		input = readline("minishell$> ");
		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		current->token = tokenize_input(input);
		current = get_command(input);
		open_fd(current, envp);
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}
