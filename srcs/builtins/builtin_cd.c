/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:48:31 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/16 16:14:22 by abonnefo         ###   ########.fr       */
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

void	print_error_cd(t_env *env, int i)
{
	if (i == 1)
	{
		write(2, "minishell: cd: ", 15);
		write(2, env->path_to_change, ft_strlen(env->path_to_change));
		write(2, ": ", 2);
		perror("");
	}
	if (i == 2)
	{
		update_env_pwd(env, env->new_directory);
		free(env->new_directory);
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

char	*get_home_directory(void)
{
	return (getenv("HOME"));
}

int	ft_builtin_cd(char **args, t_env *env)
{
	char	*home;

	if (args[1] == NULL || ft_strcmp_minishell(args[1], "~") == 0)
	{
		home = get_home_directory();
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


/* int	ft_builtin_cd(char **args, t_env *env)
{
	char	*home;

	if (args[1] == NULL || ft_strcmp_minishell(args[1], "~") == 0)
	{
		home = get_home_directory();
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
	// printf("%spath_to_change = %s%s\n", RED, env->path_to_change, RESET);
	// printf("%sPWD before = %d %s\n", YELLOW, ft_builtin_pwd(STDOUT_FILENO), RESET);
	if (chdir(env->path_to_change) == -1)
	{
		print_error_cd(env, 1);
		return (1);
	}
	env->new_directory = getcwd(NULL, 0);
	if (env->new_directory)
		print_error_cd(env, 2);
	// printf("%sPWD after = %d %s\n", MAGENTA, ft_builtin_pwd(STDOUT_FILENO), RESET);
	return (0);
}
 */