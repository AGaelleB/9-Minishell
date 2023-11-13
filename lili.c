
int	export_expander(char *var_name, char *str, t_env *env)
{
	char *ret;
	char *new;
	
	int flag;
	int i;
	int j;
	int k;
	int l;
	int m;

	new = malloc(sizeof(char) * SIZE);
	ret = malloc(sizeof(char) * SIZE);
	if (!ret)
		return (-1);
	new = str;
	i = 0;
	l = 0;
	flag = 0;
	while(str[i])
	{
		k = 0;
		var_name = extract_var_name(str);
		j = find_env_var(env, var_name);
		if (j != -1)
		{
			if (flag == 0)
			{
				flag = 1;
				while (str[i] && str[i] != '=')
					ret[l++] = str[i++];
				while (str[i] && str[i] != '$')
					ret[l++] = str[i++];
			}
			while (env->cpy_env[j][k] != '=')
				k++;
			if (env->cpy_env[j][k] == '=')
				k++;
			while(env->cpy_env[j][k])
			{
				ret[l] = env->cpy_env[j][k++];
				l++;
			}
			i++;
			while (ft_isalpha(str[i]))
				i++;
			while (str[i] && str[i] != '$')
					ret[l++] = str[i++];
			k = 0;
			m = i;
			while(str[i])
			{
				new[k] = str[i++];
				k++;
			}
			i = m;
			new[k++] = '\0';
		}
		else
		{
			add_var_env(env, i, str);
			free (var_name);
			free(ret);
			return (g_exit_status);
		}
	}
	ret[l] = '\0';
	add_var_env(env, i, ret);
	free(str);
	free (var_name);
	return (g_exit_status);
}