/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:21:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/04 16:55:53 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_access_exec(t_process_data *data, t_env *env, char *command)
{
	perror(command);
	ft_free_tab(data->command->command_arg);
	free_child(data, env);
	exit(g_exit_status = 126);
}

void	exit_access_not_found(t_process_data *data, t_env *env, char *command)
{
	perror(command);
	ft_free_tab(data->command->command_arg);
	free_child(data, env);
	exit(g_exit_status = 127);
}

int	handle_access_verification(t_process_data *data, t_env *env, char *cmd)
{
	struct stat	file_stat;

	if ((cmd[0] == '.' && cmd[1] == ' ') || (cmd[0] == '.' && !(cmd[1])))
	{
		write(2, ".: usage: . filename [arguments]\n", 34);
		ft_free_tab(data->command->command_arg);
		free_child(data, env);
		exit(g_exit_status = 2);
	}
	else if (cmd[0] == '.' && cmd[1] == '/')
	{
		if (access(cmd, F_OK))
			return (exit_access_not_found(data, env, cmd), 1);
		if (access(cmd, X_OK) && access(cmd, F_OK) == 0)
			return (exit_access_exec(data, env, cmd), 1);
		if (stat(cmd, &file_stat) == 0)
		{
			if (S_ISDIR(file_stat.st_mode))
				return (is_dir_error(cmd));
		}
		g_exit_status = 1;
		return (42);
	}
	return (g_exit_status);
}

int	verif_access(t_process_data *data, t_env *env, char *cmd)
{
	if (cmd[0] == '.' && cmd[1] == '.')
		return (check_dots_commands(data, env, cmd));
	return (handle_access_verification(data, env, cmd));
}
