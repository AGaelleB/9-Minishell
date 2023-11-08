static int	check_builtin_commands(char *args)
{
	return ((ft_strcmp_minishell(args, "unset") == 0)
		|| (ft_strcmp_minishell(args, "export") == 0)
		|| (ft_strcmp_minishell(args, "pwd") == 0)
		|| (ft_strcmp_minishell(args, "echo") == 0));
}

static void	print_no_file_or_directory(t_env *env, char *args)
{
	write(2, "minishell: ", 12);
	write(2, args, ft_strlen(args));
	write(2, ": No such file or directory", 28);
	write(2, "\n", 1);
	env->flag_error = true;
	g_exit_status = 127;
}

char	*ft_check_relative_paths(t_env *env, char *args)
{
	char	**temp_path;
	char	*valid_path;

	temp_path = ft_get_paths(env);
	if (!temp_path || temp_path[0][0] == '\0')
	{
		if (!check_builtin_commands(args))
		{
			print_no_file_or_directory(env, args);
			ft_free_tab(temp_path);
			return (NULL);
		}
	}
	env->flag_error = false;
	valid_path = find_valid_path(temp_path, args);
	ft_free_tab(temp_path);
	if (valid_path && access(valid_path, F_OK | X_OK) == 0)
		return (valid_path);
	return (NULL);
}
