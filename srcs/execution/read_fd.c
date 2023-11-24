/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/24 10:10:51 by abonnefo         ###   ########.fr       */
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
}

static void	handle_execve_processes(t_process_data *data, t_env *env)
{
	if (g_exit_status == 139)
		g_exit_status = 0;
	while (data->current)
	{
		if (pipe(data->current->fd) == -1)
			exit_with_error("pipe", data->child_pids);
		here_doc_ray(data, env);
		data->pid = fork();
		data->child_pids[data->index++] = data->pid;
		data->current->fd_in = data->current->fd[0];
		data->current->fd_out = data->current->fd[1];
		handle_all_process(data, env);
		data->current = data->current->next;
	}
}

void	execve_fd(t_command *current, t_env *env)
{
	t_process_data	data;

	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	data.current_hd = 0;
	init_execve(current, &(data.child_pids));
	data.current->flag = 0;
	data.current = current;
	handle_execve_processes(&data, env);
	wait_for_children(data.command, data.child_pids);
	if (data.count_hd)
		free(data.heredocs); // NEW FREE
	cleanup(data.child_pids, data.infile);
}
