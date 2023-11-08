/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:21:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 17:53:10 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_with_error(char *message, pid_t *child_pids)
{
	perror(message);
	free(child_pids);
	exit(1);
}

int	verif_access(t_command *current, char *command)
{
	struct stat	file_stat;

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

void	exit_access_exec(t_command *current, char *command)
{
	perror(command);
	// Add any necessary cleanup here
	(void)current;
	exit(126);
}

void	exit_access_not_found(t_command *current, char *command)
{
	perror(command);
	// Add any necessary cleanup here
	(void)current;
	exit(127);
}

int	is_dir_error(t_command *current, char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	g_exit_status = 126;
	ft_putstr_fd(": Is a directory\n", 2);
	// Add any necessary cleanup here
	(void)current;
	return (1);
}
