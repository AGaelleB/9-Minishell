
int	handle_export_args(char **args, t_env *env, t_export *export)
{
	int	arg_idx;
	int	i;

	i = 0;
	if (!args[1])
	{
		free(export);
		return (print_env_vars(env));
	}
	arg_idx = 1;
	while (args[arg_idx])
	{
		if (process_arg(export, args[arg_idx], env, &i) != 0)
		{
			free_export_str(export);
			return (g_exit_status);
		}
		arg_idx++;
	}
	return (g_exit_status);
}

int	ft_builtin_export(char **args, t_env *env)
{
	t_export	*export;

	export = malloc(sizeof(t_export));
	if (!export)
		return (0);
	export->new = NULL;
	export->ret = NULL;
	handle_export_args(args, env, export);
	// if (handle_export_args(args, env, export) == g_exit_status)
	// 	return (0);
	if (export->str && ft_strchr(export->str, '$'))
		free_export_str(export);
	else
		free_export_basic(export);
	return (g_exit_status);
}
