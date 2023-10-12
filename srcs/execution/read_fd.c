/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/12 14:26:20 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_process_data *data)
{
	close(data->current->fd_in);
	dup2(data->infile, 0);
	if (data->current->next)
		dup2(data->current->fd_out, 1);
	close(data->current->fd_out);
	ft_close_fd();
	if (data->current->fd_in != -1)
	{
		dup2(data->current->fd_in, 0);
		close(data->current->fd_in);
	}
	if (data->current->fd_out != -1)
	{
		dup2(data->current->fd_out, 1);
		close(data->current->fd_out);
	}
	open_fd(data->current);
	ft_all_builtins_verif(data->current);
	ft_set_args_and_paths(data->current, data->envp);
	// print_commands_and_tokens(current); // PRINT
	if (execve_process(data->current, data->envp) == 127)
		exit(127);
}

void	handle_parent_process(t_process_data *data)
{
	close(data->current->fd_out);
	if (data->infile != 0)
		close(data->infile);
	data->infile = data->current->fd_in;
}

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;

	i = -1;
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		++i;
		waitpid(child_pids[i], NULL, 0);
	}
	signal(SIGINT, ft_builtin_ctrl_c);
}

void	handle_all_process(t_process_data *data)
{
	if (data->pid == 0)
		handle_child_process(data);
	else if (data->pid > 0)
		handle_parent_process(data);
	else
		exit_with_error("fork", data->child_pids);
}

void	execve_fd(t_command *current, char **envp)
{
	t_process_data	data;

	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	init_execve(current, &(data.child_pids));
	data.envp = envp;
	while (data.current)
	{
		if (pipe(data.current->fd) == -1)
			exit_with_error("pipe", data.child_pids);
		data.pid = fork();
		data.child_pids[data.index++] = data.pid;
		data.current->fd_in = data.current->fd[0];
		data.current->fd_out = data.current->fd[1];
		handle_all_process(&data);
		data.current = data.current->next;
	}
	wait_for_children(data.command, data.child_pids);
	cleanup(data.child_pids, data.infile);
}

/*
RAYAN
	int status;
	int i = 0;
	printf("nombre de pipes final : %d\n", command->nb_pipes);
	while (i < command->nb_pipes)
	{
		// printf("test\n");
		waitpid(0, &status, -1);
		i++;
	}
	WEXITSTATUS(status);
*/