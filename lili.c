tu es devlopeur specilise en C tu creer un minishell simplifie qui gere lexecution des commandes et des multiple pipe
exemple de commande a gerer : cat file.txt | grep word | wc

actuellemnt lorsque jexecute ces commandes dans le terminal de mon mini shell voici ce que j'obtiens
./minishell 
minishell$> cat txt.txt | grep m
minishell$> ls
minishell$> jambon

c'est comme si les commande ne sexecutait pas, ou du moins je ne vois pas l'execution sur la sortie standard

typedef enum e_token_type {
	TYPE_CMD,
	TYPE_ARG,
	TYPE_SEPARATOR,
	TYPE_BUILTIN,
	TYPE_HEREDOC,
	TYPE_EOF
} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
} t_token;

typedef struct s_command
{
	char				*command;			// e.g. "cat test.txt"
	char				**command_arg;		// e.g. "cat"
	char				*command_path;		// e.g. /usr/bin/cat/
	int					input_fd;
	int					output_fd;
	struct s_token		*token;
	struct	s_command	*next;	// Pointer vers la prochaine commande
} t_command;

char	**split_string(const char *str, char delimiter)
{
	int i;
	int j;
	int k;
	int token_count;
	char **tokens;
	int start;
	int token_size;

	token_count = 1;
	i = 0;
	while(str[i])
	{
		if (str[i] == delimiter)
			token_count++;
		i++;
	}

	tokens = malloc((token_count + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("malloc");
		return NULL;
	}

	start = 0;
	i = 0;
	j = 0;

	while(str[j])
	{
		if (str[j] == delimiter || str[j + 1] == '\0')
		{
			if (str[j] == delimiter)
				token_size = j - start;
			else
				token_size = j - start + 1;
			tokens[i] = malloc(token_size + 1);
			if (!tokens[i])
			{
				perror("malloc");
				k = 0;
				while(k < i)
				{
					free(tokens[k]);
					k++;
				}
				free(tokens);
				return (NULL);
			}
			strncpy(tokens[i], &str[start], token_size);
			tokens[i][token_size] = '\0';
			start = j + 1;
			i++;
		}
		j++;
	}
	tokens[i] = NULL;
	return (tokens);
}

t_token *new_token(t_e_token_type e_type, char *split_value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);

	token->type = e_type;
	token->split_value = ft_strdup(split_value);
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

		if (strcmp(words[i], "|") == 0)
		else if (i == 0 ||
				(i > 0 && (strcmp(words[i - 1], "|") == 0);
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

char	**ft_get_paths(char **envp)
{
	char	*path;
	char	**all_paths;

	path = NULL;
	all_paths = NULL;
	while (*envp)
	{
		if (ft_strncmp_minishell("PATH=", *envp, 5) == 0)
		{
			path = &((*envp)[5]);
			break ;
		}
		envp++;
	}
	if (path == NULL)
		return (NULL);
	all_paths = ft_split(path, ':');
	return (all_paths);
}

char	*ft_check_absolute_path(char *args)
{
	if (ft_strchr_slash(args, '/') == 1)
	{
		if (access(args, F_OK | X_OK) == 0)
			return (ft_strdup(args));
		else
			return (NULL);
	}
	return (NULL);
}

char	*find_valid_path(char **temp_path, char *args)
{
	char	*valid_path;
	int		i;

	valid_path = NULL;
	i = 0;
	while (temp_path[i] && !valid_path)
	{
		valid_path = ft_strjoin_minishell(temp_path[i], args);
		if (access(valid_path, F_OK | X_OK) != 0)
		{
			free(valid_path);
			valid_path = NULL;
		}
		i++;
	}
	return (valid_path);
}

char	*ft_check_relative_paths(char **envp, char *args)
{
	char	**temp_path;
	char	*valid_path;

	temp_path = ft_get_paths(envp);
	if (temp_path == NULL || (temp_path[0][0]) == 0)
	{
		write(2, "No such file or directory: ", 28);
		write(2, args, ft_strlen(args));
		write(2, "\n", 1);
		return (NULL);
	}
	valid_path = find_valid_path(temp_path, args);
	ft_free_tab(temp_path);
	if (valid_path != NULL)
	{
		if (access(valid_path, F_OK | X_OK) == 0)
			return (valid_path);
	}
	ft_print_error(args);
	return (NULL);
}

char	*ft_check_paths(char **envp, char *args)
{
	char	*valid_path;

	valid_path = ft_check_absolute_path(args);
	if (valid_path != NULL)
		return (valid_path);
	valid_path = ft_check_relative_paths(envp, args);
	return (valid_path);
}

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
	if (write_fd != 1)
		dup2(write_fd, 1);
	close(read_fd);
	close(write_fd);
	ft_set_args_and_paths(current, envp);
	if ((current->command_path != NULL)
		&& (execve(current->command_path, current->command_arg, envp) == -1))
	{
		perror("Error");
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

void	open_fd(t_command *current, t_token *token, char **envp)
{
	int fd[2];
	int infile = 0;  // Par défaut, lire depuis stdin
	int outfile = 1; // Par défaut, écrire vers stdout

	while (current)
	{
		pipe(fd); // Créer un nouveau pipe pour cette commande

		// Configurer la sortie (stdout)
		if (current->next != NULL)
			current->output_fd = fd[1]; // La sortie sera l'entrée du pipe suivant
		else
			current->output_fd = outfile; // Sinon, utiliser le stdout

		// Configurer l'entrée (stdin)
		current->input_fd = infile;

		// Exécuter la commande
		exec_pipe(current, current->input_fd, current->output_fd, envp);

		// La sortie de cette commande sera l'entrée de la prochaine
		if (infile != 0)
			close(infile);
		close(current->output_fd);

		infile = fd[0]; // Jambon. Le read end du pipe devient l'entrée de la prochaine commande
		current = current->next;
		(void)token;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_command	*current;
	char		*input;

	current = NULL;
	while (1)
	{
		input = readline("minishell$> ");
		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		if(current != NULL)
		{
			current->token = tokenize_input(input);
			current = get_command(input);
			open_fd(current, current->token, envp);
		}
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}