/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_error_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 10:21:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/12/08 10:01:26 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exit_with_error(char *message, pid_t *child_pids)
{
	perror(message);
	free(child_pids);
	exit(1);
}

int	is_dir_error(char *command)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	g_exit_status = 126;
	return (42);
}

int	check_dots_commands(t_process_data *data, t_env *env, char *command)
{
	write(2, "minishell: ", 11);
	write(2, command, ft_strlen(command));
	write(2, " :command not found", 19);
	write(2, "\n", 1);
	ft_free_tab(data->command->command_arg);
	free_child(data, env);
	exit(g_exit_status = 127);
}
