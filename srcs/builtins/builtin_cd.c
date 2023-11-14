/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:48:31 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/14 14:11:21 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_env_pwd(t_env *env, char *new_pwd)
{
	env->pwd_index = find_env_var(env, "PWD");
	if (env->pwd_index != -1)
	{
		free(env->cpy_env[env->pwd_index]);
		env->cpy_env[env->pwd_index] = ft_strjoin("PWD=", new_pwd);
	}
}

void	update_env_oldpwd(t_env *env) // a tester
{
	char	*current_pwd;

	env->oldpwd_index = find_env_var(env, "OLDPWD");
	current_pwd = getcwd(NULL, 0);
	if (env->oldpwd_index != -1 && current_pwd)
	{
		free(env->cpy_env[env->oldpwd_index]);
		env->cpy_env[env->oldpwd_index] = ft_strjoin("OLDPWD=", current_pwd);
	}
	free(current_pwd);
}

char	*ft_get_env(const char *name, t_env *env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env->cpy_env[i])
	{
		if ((ft_strncmp(env->cpy_env[i], name, len) == 0)
			&& (env->cpy_env[i][len] == '='))
			return (&env->cpy_env[i][len + 1]);
		i++;
	}
	return (NULL);
}

char	*get_home_directory(t_env *env)
{
	return (ft_get_env("HOME", env));
}

char	*extract_var_name(char *var_name, )

char	*check_expender(t_env *env)
{
	int		i;
	int		j;
	int		k;
	int		l;
	int		m;
	char	*var_name;
	char	*ret;

	i = 0;
	j = 0;
	k = 0;
	l = 0;
	m = 0;
	var_name = malloc(sizeof(char) * SIZE);
	ret = malloc(sizeof(char) * SIZE);
	while (env->path_to_change[i])
	{
		if (env->path_to_change[i] == '$')
		{
			i++;
			while (env->path_to_change 
				&& (ft_isalnum(env->path_to_change[i])
				&& env->path_to_change[i] != '_'
				&& env->path_to_change[i] != ':' 
				&& env->path_to_change[i] != '$'))
			{
				var_name[j++] = env->path_to_change[i++];
			}
			j = find_env_var(env, var_name);
			if (j != -1)
			{
				while (env->cpy_env[j][k] != '=')
					k++;
				if (env->cpy_env[j][k] == '=')
					k++;
				while (env->cpy_env[j][k])
				{
					var_name[l] = env->cpy_env[j][k++];
					l++;
				}
				var_name[l] = '\0';
				l = 0;
				while (var_name[l])
					ret[m++] = var_name[l++];
			}
		}
		ret[m++] = env->path_to_change[i++];
	}
	ret[m] = '\0';
	return (ret);
}

int	ft_builtin_cd(char **args, t_env *env)
{
	char	*home;

	if (args[1] == NULL || ft_strcmp_minishell(args[1], "~") == 0)
	{
		home = get_home_directory(env);
		if (!home)
		{
			write(2, "minishell: cd: HOME not set\n", 29);
			g_exit_status = 1;
			return (g_exit_status);
		}
		env->path_to_change = home;
	}
	else
		env->path_to_change = args[1];
	update_env_oldpwd(env);
	env->path_to_change = check_expender(env);
	if (chdir(env->path_to_change) == -1)
	{
		print_error_cd(env, 1);
		return (1);
	}
	env->new_directory = getcwd(NULL, 0);
	if (env->new_directory)
		print_error_cd(env, 2);
	return (g_exit_status);
}
