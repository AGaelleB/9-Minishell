void	write_exit_simple(t_env *env, char *str)
{
	g_exit_status = 0;
	write(1, "exit\n", 5);
	free(str);
	ft_free_env(env);
	exit(g_exit_status);
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

int	ft_is_all_space(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!(input[i] >= 9 && input[i] <= 13) && input[i] != 32)
			return (0);
		i++;
	}
	// free(input);
	return (1);
}

void	ctrl_d_main(char *input, t_command *new_cmd,
	t_env *env_bis, int flag_ok)
{
	if (!input)
	{
		write(1, "exit", 5);
		write(1, "\n", 1);
		ft_close_all_fd();
		ft_free_env(env_bis);
		if (flag_ok == 1)
			ft_free_all(new_cmd, new_cmd->token_head);
		exit(0);
	}
}
