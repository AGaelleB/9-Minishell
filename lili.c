
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

int	main(int ac, char **av, char **envp)
{
	char *input;
	char *cmd_path;
	char **cmd_args;

	while (1)
	{
		input = readline("minishell$> ");

		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		//////////////////////
		cmd_args = ft_split(input, ' ');
		if (!cmd_args || !cmd_args[0])
			free(input);
		else
		{
			cmd_path = ft_check_paths(envp, cmd_args[0]);
			if (cmd_path)
			{
				if (execve(cmd_path, cmd_args, envp) == -1)
					perror("minishell"); // a gerer plus tard
				free(cmd_path);
			}
		}
		//////////////////////
		add_history(input);
		ft_free_tab(cmd_args);
		free(input);
	}
	(void)ac;
	(void)av;
	return (0);
}
