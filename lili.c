can you fix it


int	ft_builtin_write_exit(char *input)
{
	char *str;

	str = ft_strtrim(input, " ");
	if (ft_strcmp_minishell(str, "exit") == 0)
	{
		free(str);
		printf("exit\n");
		return (1);
	}
	free(str);
	return (0);
}

int	ft_builtin_enter(char *input)
{
	if (ft_strcmp_minishell(input, "") == 0)
	{
		free(input);
		return (1);
	}
	return (0);
}

int ft_is_all_space(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ')
			return (0);
		i++;
	}
	free(input);
	return (1);
}

void	ft_builtin_ctrl_D(char *input)
{
	if (!input) 
	{
		write(1, "exit\n", 5);
		exit(0);
	}
}


int	ft_all_builtins_exit(char *input)
{
	if (ft_builtin_write_exit(input) != 0)
		return (1);
	if (ft_builtin_enter(input) != 0)
		return (2);
	if (ft_is_all_space(input) != 0)
		return (2);
	return (0);
}

int main(int ac, char **av, char **envp)
{
	char		*input;
	t_command	*new_commands;

	new_commands = NULL;
	signal(SIGINT, handle_sigint);
	while (1)
	{
		input = readline("minishell$> ");
		ft_builtin_ctrl_D(input);
		if (ft_all_builtins_exit(input) == 1 )
		{
			free(input);
			exit(0);
		}
		else if (ft_all_builtins_exit(input) == 2)
		{
			free(input);
			continue;
		}
		new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			new_commands->token = tokenize_input(input);
			open_fd(new_commands, envp);
		}
		add_history(input);
		free(input);
	}
	close(new_commands->fd[1]);
	close(new_commands->fd[0]);
	ft_free_all_cmd(new_commands);
	free(new_commands);
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}

