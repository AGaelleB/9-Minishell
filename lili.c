peux tu resoudre la fin de lexecution dune commande car apres lexecve le programme ne se termine pas.
voici les exmple de commande effectuees dans mon minishell:
exemple un:
➜  minish git:(main) ✗ ./minishell
minishell$> cat txt.txt 
minishell$> coucou mots
c'est 
mémoire
mots
les mots

beaucoup de mots
^C

exemple deux :
➜  minish git:(main) ✗ ./minishell
minishell$>  cat txt.txt | grep m | wc
^C


pour l'exemple un dans la fonction void open_fd(t_command *current, t_token *token, char **envp) cette ligne permet au progranmme de s'arreter :"wait(NULL);  // Attendre la terminaison du processus enfant (optionnel)" 
mais cela ne fonctionne pas avec lexemple deux.

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
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
} t_token;

typedef struct s_command
{
	int					nb_pipes; //ATTENTION
	char				*command;			// e.g. "cat test.txt"
	char				**command_arg;		// e.g. "cat"
	char				*command_path;		// e.g. /usr/bin/cat/
	// char	separator;			// e.g. '|'
	int					read_fd;
	int					write_fd;
	struct s_token		*token;
	struct	s_command	*next;	// Pointer vers la prochaine commande
} t_command;

void count_and_set_pipes(char *input, t_command *command)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			count++;
		i++;
	}
	command->nb_pipes = count;
	// printf("nombre de pipes commande : %d\n", command->nb_pipes);
}

int	ft_strchr_slash(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_strncmp_minishell(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	while ((i < n) && (s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	if (i < n)
		return (s1[i] - s2[i]);
	return (0);
}

char	*ft_strjoin_minishell(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*dest;

	i = ft_strlen(s1) + ft_strlen(s2);
	dest = malloc(sizeof(char) * (i + 2));
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '/';
	i++;
	while (s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

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
	// printf("debutt set path and argc\n\n");
	current->command_arg = ft_split(current->command, ' ');
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
	// printf("PATH : %s\n\n", current->command_path);

}

void child_process(t_command *current, int read_fd, int write_fd, char **envp)
{
	// printf("Début child process\n");

	dup2(read_fd, 0);  // Duplique read_fd vers stdin
	if (write_fd != 1)
		dup2(write_fd, 1);  // Duplique write_fd vers stdout
   
	ft_set_args_and_paths(current, envp);  // Met à jour args et paths
	
	if (current->command_path == NULL)
	{
		write(2, "command not found: ", 19);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, "\n", 1);
		exit(127);  // Code de sortie standard pour "commande introuvable"
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1) 
	{
		perror("Error");
		exit(-1);
	}
}

void open_fd(t_command *current, t_token *token, char **envp)
{
	int fd[2];
	int infile = 0;
	pid_t pid;
	t_command *command;
	command = current;

	while (current)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		// Configurer l'entrée (stdin)
		current->read_fd = infile;
		if (current->next != NULL)
			current->write_fd = fd[1];
		else
			current->write_fd = 1; // stdout
		pid = fork();
		if (pid == 0)
		{
			// Enfant
			//close(fd[0]); // Fermer l'extrémité de lecture inutilisée dans l'enfant
			child_process(current, current->read_fd, current->write_fd, envp);
		}
		else if (pid > 0) 
		{
			// Parent
			// wait(NULL);  // Attendre la terminaison du processus enfant (optionnel)
			close(fd[1]); // Fermer l'extrémité d'écriture inutilisée dans le parent
			if (current->next != NULL)
				infile = fd[0]; // Pour la prochaine itération
			else
				close(fd[0]); // Fermer aussi l'extrémité de lecture si c'est la dernière commande
		} 
		else
		{
			perror("fork");
			exit(1);
		}
		current = current->next;
	}

	int status;
	int i = 0;
	// printf("nombre de pipes final : %d\n", command->nb_pipes);
	while (i < command->nb_pipes)
	{
		waitpid(0, &status, -1);
	}
	WEXITSTATUS(status);
	(void)token;
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_command	*new_commands;

	while (1)
	{
		input = readline("minishell$> ");
		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			new_commands->token = tokenize_input(input);
			open_fd(new_commands, new_commands->token, envp);
		}
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}