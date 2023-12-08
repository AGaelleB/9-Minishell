/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/08 13:22:49 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;
	int	status;

	i = -1;
	signal(SIGINT, SIG_IGN);
	while (++i <= command->nb_pipes)
	{
		if (waitpid(child_pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = WTERMSIG(status) + 128;
		}
	}
	signal(SIGINT, ctrl_c_main);
	signal(SIGQUIT, SIG_IGN);
}

int	handle_execve_processes(t_process_data *data, t_env *env)
{
	if (g_exit_status == 139)
		g_exit_status = 0;
	while (data->current)
	{
		if (pipe(data->current->fd) == -1)
			exit_with_error("pipe", data->child_pids);
		here_doc_ray(data, env);
		if (g_exit_status == 24)
		{
			signal(SIGINT, ctrl_c_main);
			signal(SIGQUIT, SIG_IGN);
			return (g_exit_status = 130);
		}
		data->pid = fork();
		data->child_pids[data->index++] = data->pid;
		data->current->fd_in = data->current->fd[0];
		data->current->fd_out = data->current->fd[1];
		handle_all_process(data, env);
		data->current = data->current->next;
	}
	return (0);
}

void	execve_fd(t_command *current, t_env *env)
{
	t_process_data	data;

	return_data(&data);
	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	data.current_hd = 0;
	init_execve(current, &(data.child_pids));
	data.current->flag = 0;
	data.current = current;
	data.execute_builtin_later = 0;
	if (handle_execve_processes(&data, env) == 130)
	{
		free(data.heredocs);
		cleanup(data.child_pids, data.infile);
		return ;
	}
	wait_for_children(data.command, data.child_pids);
	cleanup(data.child_pids, data.infile);
}
