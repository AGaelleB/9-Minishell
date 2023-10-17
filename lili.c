int	ft_builtin_export(char **args, t_env *env)
{
	int		i;
	int		arg_idx;
	char	*str;
	int		j;
	int		k;
	
	i = 0;
	j = 0;
	k = 0;
	arg_idx = 1;	
	str = malloc(sizeof(char) * (ft_strlen(args[arg_idx]) + 1));
	if (!str)
		return (-1);
	env->flag_builtin = true;
	if (!args[1])
	{
		while (env->cpy_env[i])
		{
			ft_putstr_fd("export ", STDOUT_FILENO);
			ft_putstr_fd(env->cpy_env[i], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
			i++;
		}
		return (1);
	}
	while (args[arg_idx])
	{
		i = 0;
		while (env->cpy_env[i])
		{
			j = 0;
			k = 0;
			while(args[arg_idx][j] != '=' && args[arg_idx][j])
			{
				str[k] = args[arg_idx][j];
				j++;
				k++;
			}
			str[k] = '\0';
			if (ft_strncmp(env->cpy_env[i], str, ft_strlen(str)) == 0
				&& env->cpy_env[i][ft_strlen(str)] == '=')
			{
				free(env->cpy_env[i]);
				while (env->cpy_env[i + 1])
				{
					env->cpy_env[i] = env->cpy_env[i + 1];
					i++;
				}
				env->cpy_env[i] = args[arg_idx];
				env->cpy_env[i + 1] = NULL;
				free(str);
				return (1);
			}
			i++;
		}
		arg_idx++;
	}
	i = 0;
	arg_idx = 1;
	while (env->cpy_env[i])
	{
		if(env->cpy_env[i + 1] == NULL)
		{
			printf("arg : %s\n", args[arg_idx]);
			env->cpy_env[i + 1] = args[arg_idx];
			env->cpy_env[i + 2] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}