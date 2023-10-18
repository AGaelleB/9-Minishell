peux tu implementer les builtins suivant de la meme facons que ceux que jai actulement :
				env_bis->flag_builtin = false;
				new_commands->command_arg = parse_input_quote(new_commands->command);
				if (ft_strncmp(new_commands->command, "unset", 5) == 0)
					ft_builtin_unset(new_commands->command_arg, env_bis);
				else if (ft_strcmp_minishell(new_commands->command, "cd") == 0)
					ft_builtin_cd(new_commands->command_arg, env_bis);
				else if (ft_strncmp(new_commands->command, "cd ", 3) == 0)
					ft_builtin_cd(new_commands->command_arg, env_bis);
				else if (ft_strncmp(new_commands->command, "export", 6) == 0)
					ft_builtin_export(new_commands->command_arg, env_bis);

dans ce code la stp :

int	error_input(char *input)
{
	if (ft_builtin_enter(input) != 0)
		return (2);
	if (ft_builtin_write_exit(input) != 0)
	{
		free(input);
		exit(0);
	}
	if (ft_is_all_space(input) != 0)
		return (2);
	if (ft_strcmp_minishell(input, "\"\"") == 0
		|| ft_strcmp_minishell(input, "\'\'") == 0)
	{
		printf("minishell: : command not found\n");
		free(input);
		return (2);
	}
	return (0);
}

static int	check_pwd(t_command *current)
{
	if (ft_strcmp_minishell(current->command, "pwd") == 0)
	{
		ft_builtin_pwd(STDOUT_FILENO);
		return (1);
	}
	return (0);
}

static int	check_echo(t_command *current, t_env *env)
{
	char	**tab;

	if (ft_strncmp(current->command, "echo ", 5) == 0)
	{
		tab = parse_input_quote_echo(env, current->command);
		ft_builtin_echo(tab);
		return (1);
	}
	return (0);
}

static int	check_env(t_command *current, t_env *env)
{
	if (ft_strcmp_minishell(current->command, "env") == 0)
	{
		ft_builtin_env(env);
		return (1);
	}
	return (0);
}

int	builtins_verif(t_command *current, t_env *env)
{
	while (current)
	{
		if (check_pwd(current))
			return (1);
		if (check_echo(current, env))
			return (1);
		if (check_env(current, env))
			return (1);
		current = current->next;
	}
	return (0);
}
