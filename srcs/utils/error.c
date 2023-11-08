/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:21:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 10:40:25 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		g_exit_status = 1;
	}
	if (i == 2)
	{
		update_env_pwd(env, env->new_directory);
		g_exit_status = 0;
		free(env->new_directory);
	}
}

void exit_access_exec(t_command *current, char *command) // NEW
{
	perror(command);
	// Add any necessary cleanup here
	(void)current;
	exit(126);
}

void exit_access_not_found(t_command *current, char *command) // NEW
{
	perror(command);
	// Add any necessary cleanup here
	(void)current;
	exit(127);
}

int is_dir_error(t_command *current, char *command) // NEW
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	g_exit_status = 126;
	ft_putstr_fd(": Is a directory\n", 2);
	// Add any necessary cleanup here
	(void)current;
	return (1);
}

int verif_access(t_command *current, char *command) // NEW
{
	(void)current;
	struct stat file_stat;
	if (command[0] == '.' && command[1] == '/')
	{
		if (access(command, F_OK))
		{
			exit_access_not_found(current, command);
			return (1); 
		}
		if (access(command, X_OK) && access(command, F_OK) == 0)
		{
			exit_access_exec(current, command);
			return (1);
		}
		if (stat(command, &file_stat) == 0)
		{
			if (S_ISDIR(file_stat.st_mode))
				return (is_dir_error(NULL, command));
		}
		return (0);
	}
	return (0);
}

