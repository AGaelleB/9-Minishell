
Je suis en train d'essayer d'implementer la gestion des quotes simples ' ' dans mon minishell 
je crois que le fait que je split sur les espaces creer mon probleme car lorsque j execute la commane "echo 'je veux des                            espaces' comme ca" j'ai comme resultat "je veux des espaces' comme ca".
comment resoudre ce probleme ? est-ce que je dois faire un split sur les '\'' ? ou y a t il une autre solution ?

l'execution : 

./minishell 
minishell$> echo 'je veux des                            espaces' comme ca
command : echo 
command : echo je veux des                            espaces
command : echo je veux des                            espaces comme ca
Command 0: echo je veux des                            espaces comme ca
*****notre echo*****


child_process - command_arg[0] = echo
child_process - command_arg[1] = je
child_process - command_arg[2] = veux
child_process - command_arg[3] = des
child_process - command_arg[4] = espaces
child_process - command_arg[5] = comme
child_process - command_arg[6] = ca
je veux des espaces comme ca
minishell$> 

mon code :

typedef struct s_quote 
{
	char	*str;
	struct	s_quote  *next;
} t_quote;

typedef struct s_command
{
	int					nb_pipes;
	char				*command;	// e.g. "cat test.txt"
	char				**command_arg;	// e.g. "cat"
	char				*command_path;	// e.g. /usr/bin/cat/
	int					fd[2];
	int					fd_out;
	int					fd_in;
	char				*file_name;
	struct s_token		*token_head;
	struct s_quote		*quote_head;
	struct s_command	*next;
} t_command;

void	ft_set_args_and_paths(t_command *current, char **envp)
{
	current->command_arg = NULL;
	current->command_path = NULL;
	current->command_arg = ft_split(current->command, ' ');
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
}

int	child_process(t_command *current, char **envp)
{
	ft_set_args_and_paths(current, envp);
	int i = 0;
	while(current->command_arg[i])
	{
		printf("child_process - command_arg[%d] = %s\n", i, current->command_arg[i]);
		i++;
	}
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
	printf("J'ai execve dans le child_process\n");
	return (0);
}

t_quote	*add_str_to_list(t_quote *list, char *str)
{
	t_quote	*new_list_str;
	t_quote	*temp;
	
	new_list_str = malloc(sizeof(t_quote));
	if (!new_list_str)
		exit(1);
	
	new_list_str->str = str;
	new_list_str->next = NULL;
	if (!list)
		return (new_list_str);
	
	temp = list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_list_str;

	// printf("%snew_list_str->str: %s%s\n", CYAN, new_list_str->str, RESET); // PRINT

	return (list);
}

t_quote	*parse_input_quote(char *input, char **envp)
{
	t_quote	*substr_list;
	char	*start;
	bool	flag_open_quote;
	
	substr_list = NULL;
	start = input;
	flag_open_quote = false;
	while (*input)
	{
		if (*input == '\'')
		{
			if (flag_open_quote == true)
			{
				*input = '\0';
				substr_list = add_str_to_list(substr_list, start);
				start = input + 1;
				flag_open_quote = false;
			}
			else
			{
				if (start != input)
				{
					*input = '\0';
					substr_list = add_str_to_list(substr_list, start);
				}
				start = input + 1;
				flag_open_quote = true;
			}
		}
		input++;
	}
	if (start != input)
		substr_list = add_str_to_list(substr_list, start);
	ft_cat_list_quote(substr_list, envp);
	return (substr_list);
}

void ft_cat_list_quote(t_quote *substr_list, char **envp)
{
	t_command	*new_commands;
	char		*command;
	char		*temp;

	new_commands = NULL;
	command = NULL;
	temp = NULL;
	
	while (substr_list)
	{
		temp = ft_strjoin(command, substr_list->str);
		free(command);
		command = temp;
		// printf("%ssubstr_list->str : %s%s\n", MAGENTA, substr_list->str, RESET); // PRINT
		printf("%scommand : %s%s\n", MAGENTA, command, RESET); // PRINT
		substr_list = substr_list->next;
	}
	new_commands = get_command(command);
	count_and_set_pipes(command, new_commands);
	
	ft_all_builtins_verif(new_commands);

	// print_commands_and_tokens(new_commands);
	if(new_commands != NULL)
		execve_fd(new_commands, envp);

	free(command);
}

int	ft_builtin_echo(char **str)
{
	printf("*****notre echo*****\n");
	int	i;
	str++;
	i = 0;
	while (str[i])
	{
		printf("%s", str[i]);
		if (str[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
	return (0);
}

void	ft_all_builtins_verif(t_command *current)
{
	int	cmd_count;

	cmd_count = 0;
	while (current)
	{
		printf("%sCommand %d: %s%s\n", GREEN, cmd_count, current->command, RESET);
		
		if (ft_strncmp(current->command, "pwd", 3) == 0)
			ft_builtin_pwd();
		if (ft_strncmp(current->command, "echo", 4) == 0)
		{
			current->command_arg = ft_split(current->command, '\'');
			ft_builtin_echo(current->command_arg);
			// exit (-1);
		}
		cmd_count++;
		current = current->next;
		printf("\n");  // Just to separate different commands
	}
}
