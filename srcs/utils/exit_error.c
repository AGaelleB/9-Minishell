/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:21:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 16:12:42 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_access_exec(t_command *current, char *command)
{
	perror(command);
	// Add any necessary cleanup here
	(void)current;
	exit(g_exit_status = 126);
}

void	exit_access_not_found(t_command *current, char *command)
{
	perror(command);
	// Add any necessary cleanup here
	(void)current;
	exit(g_exit_status = 127);
}

int	is_dir_error(t_command *current, char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	// Add any necessary cleanup here
	(void)current;
	g_exit_status = 126;
	return (1);
}

int	check_dots_commands(char *command)
{
	write(2, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, " :command not found", 19);
	write(2, "\n", 1);
	exit(g_exit_status = 127);
}

int	verif_access(t_command *current, char *command)
{
	struct stat	file_stat;

	if (command[0] == '.' && command[1] == '.')
		return (check_dots_commands(command));
	if ((command[0] == '.' && command[1] == ' ') || (command[0] == '.'
			&& !(command[1])))
	{
		write(2, ".: usage: . filename [arguments]", 33);
		write(2, "\n", 1);
		exit(g_exit_status = 2);
	}
	else if (command[0] == '.' && command[1] == '/')
	{
		if (access(command, F_OK))
			return (exit_access_not_found(current, command), 1);
		if (access(command, X_OK) && access(command, F_OK) == 0)
			return (exit_access_exec(current, command), 1);
		if (stat(command, &file_stat) == 0)
		{
			if (S_ISDIR(file_stat.st_mode))
				return (is_dir_error(NULL, command));
		}
		return (g_exit_status);
	}
	return (g_exit_status);
}
