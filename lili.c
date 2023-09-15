We want to code the rediction of ">" in a fileout, we did that and we dont knwo how to spell our file in the line int fd_out = open("argc fileout", O_RDONLY);

typedef enum e_token_type
{
	TYPE_CMD,
	TYPE_ARG,
	TYPE_SEPARATOR,
	TYPE_REDIR_OUT,		// ">"
	TYPE_REDIR_IN,		// "<"
	TYPE_DELIMITATOR,	// "<<"
	TYPE_REDIR_APPEND,	// ">>"
	// TYPE_BUILTIN,
	// TYPE_HEREDOC,
	// TYPE_EOF
} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
} t_token;

typedef struct s_sigset
{

} t_sigset;

typedef struct s_command
{
	int					nb_pipes; //ATTENTION
	char				*command;			// e.g. "cat test.txt"
	char				**command_arg;		// e.g. "cat"
	char				*command_path;		// e.g. /usr/bin/cat/
	int					fd[2];
	struct s_token		*token;
	struct	s_command	*next;	// Pointer to the next command
} t_command;


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

	words = split_string(input, ' ');
	head = NULL;
	curr = NULL;
	i = 0;
	while (words[i])
	{
		token = NULL;
		if (ft_strcmp_minishell(words[i], "|") == 0)
			token = new_token(TYPE_SEPARATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], ">") == 0)
			token = new_token(TYPE_REDIR_OUT, words[i]);
		else if (ft_strcmp_minishell(words[i], "<") == 0)
			token = new_token(TYPE_REDIR_IN, words[i]);
		else if (ft_strcmp_minishell(words[i], ">>") == 0)
			token = new_token(TYPE_DELIMITATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], "<<") == 0)
			token = new_token(TYPE_REDIR_APPEND, words[i]);
		else if (i == 0 ||
				(i > 0 && (ft_strcmp_minishell(words[i - 1], "|") == 0 ||
							ft_strcmp_minishell(words[i - 1], ">") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<") == 0 ||
							ft_strcmp_minishell(words[i - 1], ">>") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<<") == 0)))
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
		// printf("decoupage : %s\n", words[i]);
		i++;
	}
	ft_free_tab(words);
	ft_free_tokens(head);
	return (head);
}


int	child_process(t_command *current, char **envp)
{
	if (current->fd[0] != -1)
	{
		dup2(current->fd[0], 0);
		close(current->fd[0]);
	}
	if (current->fd[1] != -1)
	{
		dup2(current->fd[1], 1);
		close(current->fd[1]);
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


void open_fd_pipe(t_command *current, int infile)
{
	close(current->fd[0]);
	dup2(infile, 0);
	if (current->next)
		dup2(current->fd[1], 1);
	close(current->fd[1]);
	close_fd();
}

void open_fd_redir_out(t_command *current, int infile)
{
	int fd_out = open("argc fileout", O_RDONLY);
	close(current->fd[0]);
	dup2(infile, 0);
	if (current->next)
		dup2(current->fd[1], 1);
	else
		dup2(fd_out, 1);
	close(current->fd[1]);
	close_fd();
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
		if (pid == 0) // Child
		{
			if(TYPE_DELIMITATOR)
				open_fd_pipe(current, infile);
			if(TYPE_REDIR_OUT)
				open_fd_redir_out(current, infile);

			if(child_process(current, envp) == 127)
			{
				free(child_pids);
				exit(127);
			}
		}
		else if (pid > 0) // Parent
		{
			close(current->fd[1]);
			if (infile != 0)
				close(infile);
			infile = current->fd[0];
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
