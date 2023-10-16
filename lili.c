we have a problem we are using getenv to set HOME in ft_builtin_cd but we are working on a copy of our env (env->cpy_env)
so we want to code ft_get_env to set home with our copy of env in the structure
can you help us please and tell us step by step how to do it ?


typedef struct s_env
{
	char				**cpy_env;
	char				*str;
	bool				flag_error;
	bool				flag_builtin;
	int					pwd_index;
	int					oldpwd_index;
	char				*path_to_change;
	char				*new_directory;
	struct s_env		*next;
} t_env;

char	*get_home_directory(void)
{
	return (getenv("HOME"));
}

int	ft_builtin_cd(char **args, t_env *env)
{
	char	*home;

	env->flag_builtin = true;
	if (args[1] == NULL || ft_strcmp_minishell(args[1], "~") == 0)
	{
		printf("if ~\n");
		home = get_home_directory();
		printf("HOME : %s\n", home);
		if (!home)
		{
			printf("no home\n");
			write(2, "minishell: cd: HOME not set\n", 29);
			return (1);
		}
		env->path_to_change = home;
	}
	else
		env->path_to_change = args[1];
	update_env_oldpwd(env);
	if (chdir(env->path_to_change) == -1)
	{
		print_error_cd(env, 1);
		return (1);
	}
	env->new_directory = getcwd(NULL, 0);
	if (env->new_directory)
		print_error_cd(env, 2);
	return (0);
}