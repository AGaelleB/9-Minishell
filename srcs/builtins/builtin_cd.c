/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:48:31 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/17 16:03:28 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_env_pwd(t_env *env, char *new_pwd)
{
	if (env->pwd_index != -1)
	{
		free(env->cpy_env[env->pwd_index]);
		env->cpy_env[env->pwd_index] = ft_strjoin("PWD=", new_pwd);
	}
}

void	update_env_oldpwd(t_env *env)
{
	char	*current_pwd;

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

int	ft_builtin_cd(char **args, t_env *env)
{
	char	*home;

	env->flag_builtin = true;
	if (args[1] == NULL || ft_strcmp_minishell(args[1], "~") == 0)
	{
		home = get_home_directory(env);
		if (!home)
		{
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
