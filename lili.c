Pourquoi je n'arrive pas a faire le malloc dans la fonction parse_input_quote

minishell$> echo a b c 'd           e'   f
*****notre echo*****
echo a b c 'd           e'   f

ft_set_args_and_paths
***** 1parse_input_quote DEBUT *****
***** 2parse_input_quote DEBUT *****
***** 1 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** 2 count_args DEBUT *****
***** count_args = 6 *****
minishell$> 

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
		if (!new_cmd)
		{
			perror("Failed to allocate memory for new command");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->command = NULL;
		new_cmd->command = ft_strdup(command[i]);
		new_cmd->token_head = tokenize_input(new_cmd->command);
		if (!new_cmd->command)
		{
			perror("Failed to duplicate command string");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->next = NULL;
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
	ft_free_tab(command);
	return (head);
}

void ft_set_args_and_paths(t_command *current, char **envp)
{
	printf("ft_set_args_and_paths\n");

	current->command_arg = NULL;
	current->command_path = NULL;
	// current->command_arg = parse_input_quote(current->command); // Use parse_input_quote here
	// current->command_arg = ft_split(current->command, ' ');
	current->command_arg = parse_input_quote(current->command); // Use parse_input_quote here
	int i = 0;
	while(current->command_arg[i])
	{
		printf("command_arg[%d] = %s\n", i, current->command_arg[i]);
		i++;
	}
	printf("ft_set_args_and_paths FINN\n");
   
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


int count_args(char *input)
{
	printf("***** 1 count_args DEBUT *****\n");
	int		count;
	bool	in_quote;
	bool	was_space; // pour compter le premier mot même s'il n'y a pas d'espace devant
	
	count = 0;
	in_quote = false;
	was_space = true; 
	while (*input)
	{
		printf("***** 2 count_args DEBUT *****\n");
		if (*input == '\'')
			in_quote = !in_quote;
		if (!in_quote && *input == ' ')
			was_space = true;
		else if (was_space)
		{
			count++;
			was_space = false;
		}
		input++;
	}
	return (count);
}

char	**parse_input_quote(char *input)
{
	printf("***** 1parse_input_quote DEBUT *****\n");
	t_quote	*quote;
	int		arg_count;
	char	*temp_input;
	bool	in_quote;
	printf("***** 2parse_input_quote DEBUT *****\n");
	quote = NULL;
	arg_count = count_args(input);
	printf("***** count_args = %d *****\n", arg_count);
	quote->args = malloc((arg_count + 1) * sizeof(char *));
	printf("***** 3parse_input_quote DEBUT *****\n");
	if (!quote->args)
	{
		printf("***** 4parse_input_quote DEBUT *****\n");
		return (NULL);
	}
	printf("***** 5parse_input_quote DEBUT *****\n");
	temp_input = input;
	in_quote = false;
	
	while (*input)
	{
		if (*input == '\'')
			in_quote = !in_quote;
		
		// Si nous trouvons un espace et que nous ne sommes pas entre des guillemets, 
		// ou si nous sommes à la fin de la chaîne, alors nous avons trouvé un argument
		if ((!in_quote && *input == ' ') || *(input + 1) == '\0')
		{
			if (*(input + 1) == '\0' && *input != ' ')
				input++; // Incluez le dernier caractère si ce n'est pas un espace
			*input = '\0';
			if (*temp_input != '\0') // Ne pas ajouter des chaînes vides
				quote->args[arg_count++] = strdup(temp_input);
			temp_input = input + 1;
		}
		input++;
	}
	quote->args[arg_count] = NULL;
	return (quote->args);
}

int main(int ac, char **av, char **envp)
{
	char		*input;
	int			builtin_status;
	if (ac != 1)
		return (printf("run ./minishell without arg\n"));
	if (!envp[0])
		return (printf("env is missing\n"));
	signal(SIGINT, ft_signal_ctrl_C);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell$> ");
		ft_builtin_ctrl_D(input);
		builtin_status = ft_all_builtins(input);
		if (builtin_status == 1)
		{
			free(input);
			exit(0);
		}
		else if (builtin_status == 2)
			continue;
		if (verif_nb_single_quote(input) != 0)
		{
			printf("minishell: miss quote\n");
			free(input);
			continue;
		}
		add_history(input);
		// parse_input_quote(input, envp);
		t_command *new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		ft_all_builtins_verif(new_commands);

		// print_commands_and_tokens(new_commands); // PRINT
		if(new_commands != NULL)
			execve_fd(new_commands, envp);


		
		free(input);
	}
	(void)av;
	return (0);
}
