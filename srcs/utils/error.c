/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:21:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 10:04:01 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_status;

void	ft_print_error(char *str)
{
	int	len;

	if (!ft_strncmp(str, "cd", ft_strlen(str)))
		return ;
	len = ft_strlen(str);
	write(2, "command not found :", 19);
	write(2, str, len);
	write(2, "\n", 1);
}

void	exit_with_error(char *message, pid_t *child_pids)
{
	perror(message);
	free(child_pids);
	exit(1);
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

void	exit_access(t_command *current, char *command)
{
	perror(command);
	//free
	(void)current;
	exit(126);
}

void	exit_access_2(t_command *current, char *command)
{
	perror(command);
	//free
	(void)current;
	exit(127);
}


int	is_dir_error(t_command *current, char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	g_exit_status = 126;
	ft_putstr_fd(": Is a directory\n", 2);
	//free ??? ici ou avant le exit(126) de lexecve ?
	(void)current;
	return (1);
}

int	verif_access(t_command *current, char *command)
{
	if (command[0] == '.' && command[1] == '/')
	{
		if (access(command, F_OK) == 0) // Add == 0 to check for file existence
			return (is_dir_error(current, command));
		if (access(command, X_OK | F_OK) == 0)
			return (0);
		if (access(command, X_OK) && access(command, F_OK) == 0)
		{
			exit_access(current, command);
			return (1);
		}
		if (access(command, X_OK))
		{
			exit_access_2(current, command);
			return (1);
		}
	}
	return (0);
}
