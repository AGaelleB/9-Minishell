je veux lorsque la var est existante la supprimer
la creation ensuite est deja effective:
minishell$> export PATH=NULL
minishell$> env | grep PATH
XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
MANDATORY_PATH=/usr/share/gconf/ubuntu.mandatory.path
DEFAULTS_PATH=/usr/share/gconf/ubuntu.default.path
PATH=/mnt/nfs/homes/abonnefo/bin:/mnt/nfs/homes/abonnefo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session16
PATH=NULL

static int	is_first_char_valid(char c)
{
	return (ft_isalpha(c) || c == '_' || c == '$'); // tempo pour le dollard a retirer
}

static int	is_char_valid(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '='
		|| c == '\'' || c == '\"' || c == ' ' 
		||  c == '$' ||  c == ':');
}
static void	print_invalid_identifier(char *str, int index)
{
	ft_putstr_fd("minishell: export: \'", 2);
	write(2, &str[index], 1);
	ft_putstr_fd("\': not a valid identifier\n", 2);
}

int	check_valid_identifier_export(char *str)
{
	int	i;

	i = 0;
	if (!is_first_char_valid(str[i]))
	{
		print_invalid_identifier(str, i);
		g_exit_status = 1;
		return (g_exit_status);
	}
	i++;
	while (str[i])
	{
		if (!is_char_valid(str[i]))
		{
			print_invalid_identifier(str, i);
			g_exit_status = 1;
			return (g_exit_status);
		}
		i++;
	}
	return (0);
}

char *extract_var_name(char *str)
{
	char *start;
	char *var_name;
	int len;

	start = ft_strchr(str, '$');
	if (!start)
		return NULL;
	len = 0;
	while (start[len] && (isalnum(start[len]) || start[len] == '_'
		|| start[len] == ':' || start[len] == '$'))
		len++;
	var_name = malloc(len + 1);
	if (!var_name)
		return NULL;
	strncpy(var_name, start, len); // a faire strcpy
	var_name[len] = '\0';
	return (var_name);
}

int	find_env_var(t_env *env, char *arg)
{
	int	i;

	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], arg, ft_strlen(arg)) == 0
			&& env->cpy_env[i][ft_strlen(arg)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	remove_env_var(t_env *env, int idx)
{
	free(env->cpy_env[idx]);
	while (env->cpy_env[idx + 1])
	{
		env->cpy_env[idx] = env->cpy_env[idx + 1];
		idx++;
	}
	env->cpy_env[idx] = NULL;
}

void	ft_builtin_unset(char **args, t_env *env)
{
	int	idx;
	int	arg_idx;

	arg_idx = 1;
	if (!args[1])
		return ;
	while (args[arg_idx])
	{
		idx = find_env_var(env, args[arg_idx]);
		if (idx != -1)
			remove_env_var(env, idx);
		arg_idx++;
	}
	g_exit_status = 0;
}

char	*create_str_from_arg(char *arg)
{
	int		i;
	char	*str;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, arg, i + 1);
	return (str);
}

int	update_var_env(t_env *env, char *arg)
{
	int		i;
	char	*str;

	str = create_str_from_arg(arg);
	if (!str)
		return (-1);
	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], str, ft_strlen(str)) == 0
			&& env->cpy_env[i][ft_strlen(str)] == '=')
		{
			free(env->cpy_env[i]);
			env->cpy_env[i] = ft_strdup(arg);
			env->cpy_env[i + 1] = NULL;
			free(str);
			return (1);
		}
		i++;
	}
	free(str);
	return (0);
}

int	add_var_env(t_env *env, int i, char *str)
{
	i = 0;
	while (env->cpy_env[i])
	{
		if (env->cpy_env[i + 1] == NULL)
		{
			env->cpy_env[i + 1] = str;
			env->cpy_env[i + 2] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

void	print_export(char *str, int fd)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (str[i])
	{
		ft_putchar_fd(str[i], fd);
		if (str[i] == '=' && flag == 0)
		{
			flag = 1;
			ft_putchar_fd('\"', fd);
		}
		i++;
	}
	if (flag == 1)
		ft_putchar_fd('\"', fd);
}

static int	print_env_vars(t_env *env)
{
	int	i;

	i = 0;
	while (env->cpy_env[i])
	{
		ft_putstr_fd("export ", STDOUT_FILENO);
		print_export(env->cpy_env[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (1);
}

int	ft_builtin_export(char **args, t_env *env)
{
	int		arg_idx;
	int		i;
	char	*str;

	i = 0;
	if (!args[1])
		return (print_env_vars(env));
	arg_idx = 1;
	while (args[arg_idx])
	{
		str = handle_quotes_export(args[arg_idx]);
		if (check_valid_identifier_export(str))
		{
			if (update_var_env(env, str) == 1)
				return (1);
			arg_idx++;
		}
		else
			break ;
	}
	add_var_env(env, i, str);
	return (g_exit_status);
}
