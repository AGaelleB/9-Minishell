t_export	*init_export(void)
{
	t_export	*export;

	export = malloc(sizeof(t_export));
	if (!export)
		return (NULL);
	export->ret = malloc(sizeof(char) * SIZE);
	if (!export->ret)
		return (NULL);
	export->new = malloc(sizeof(char) * SIZE);
	if (!export->new)
		return (NULL);
	export->flag = 0;
	export->i = 0;
	export->j = 0;
	export->k = 0;
	export->l = 0;
	export->m = 0;
	return (export);
}

void	begin_var(t_export *export, char *str)
{
	if (export->flag == 0)
	{
		export->flag = 1;
		while (str[export->i] && str[export->i] != '=')
			export->ret[export->l++] = str[export->i++];
		while (str[export->i] && str[export->i] != '$')
			export->ret[export->l++] = str[export->i++];
	}
}

void	copy_env_in_return(t_export *export, t_env *env)
{
	while (env->cpy_env[export->j][export->k] != '=')
		export->k++;
	if (env->cpy_env[export->j][export->k] == '=')
		export->k++;
	while (env->cpy_env[export->j][export->k])
	{
		export->ret[export->l] = env->cpy_env[export->j][export->k++];
		export->l++;
	}
	export->i++;
}

char	*re_init_var_str(t_export *export, char *str)
{
	while (ft_isalpha(str[export->i]))
		export->i++;
	while (str[export->i] && str[export->i] != '$')
			export->ret[export->l++] = str[export->i++];
	export->k = 0;
	export->m = export->i;
	while (str[export->i])
	{
		export->new[export->k] = str[export->i++];
		export->k++;
	}
	export->i = export->m;
	export->new[export->k++] = '\0';
	if (ft_is_all_space(export->new))
		return (NULL);
	// printf("%s NEW : %s\n%s", MAGENTA, export->new, RESET);
	return	(export->new);
	
}

char	*check_none_var(char *str)
{
	char	*str_cpy;
	int		i;
	int		y;

	i = 0;
	y = 0;
	str_cpy = malloc(sizeof(char) * (ft_strlen(str) + 1));
	while(str[i])
	{
		if(str[i] == '$')
		{
			i++;
			while (ft_isalnum(str[i]))
				i++;
		}
		str_cpy[y++] = str[i++];
	}
	return(str_cpy);
}

int	export_expander(t_export *export, char *var_name, char *str, t_env *env)
{
	char	*str_cpy;
	

	str_cpy = ft_strdup(str);
	while (1)
	{
		export->k = 0;
		// printf("%s STR : %s\n%s", BLUE, str, RESET);
		var_name = extract_var_name(str);
		// printf("%svar_name : %s\n%s", MAGENTA, var_name, RESET);
		export->j = find_env_var(env, var_name);
		if (export->j != -1)
		{
			begin_var(export, str);
			copy_env_in_return(export, env);
			str = re_init_var_str(export, str);
			// printf("%s STR[%d] : %s\n%s", GREEN, export->i, str, RESET);
			if (!str)
			{
				// printf("je suis null je quit\n");
				update_var_env(env, str_cpy);
				export->ret[export->l] = '\0';
				add_var_env(env, export->i, export->ret);
				free(str);
				free (var_name);
				// faire free de la struct export
				return (g_exit_status);
			}
		}
		else
		{
			str = check_none_var(str);
			add_var_env(env, export->i, str);
			free (var_name);
			// faire free de la struct export
			return (g_exit_status);
		}
	}
	// printf("%s STR[%d] : %s\n%s", YELLOW, export->i, str, RESET);
	// update_var_env(env, str_cpy);
	// export->ret[export->l] = '\0';
	// add_var_env(env, export->i, export->ret);
	// free(str);
	// free (var_name);
	// faire free de la struct export
	return (g_exit_status);
}