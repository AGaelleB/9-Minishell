/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/01 11:40:59 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;
	int	status; // AJOUT
	// int	exit_status; // AJOUT
	// int	term_signal; // AJOUT
	i = -1;
	// i = 0;
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		++i;
		waitpid(child_pids[i], &status, 0); // AJOUT
		// i++;
	}
	// if (WIFEXITED(status))
	// 	exit_status = WEXITSTATUS(status);
	// if (WIFSIGNALED(status))
	// 	term_signal = WTERMSIG(status);
	signal(SIGINT, ft_builtin_ctrl_c);
}

static void	handle_heredoc_tokens(t_process_data *data)
{
	t_token	*token;
	pid_t	heredoc_pid;
	int		flag;

	token = data->current->token_head;
	flag = 0;
	while (data->current && flag == 0)
	{
		while (token && flag == 0)
		{	
			if (token->type == TYPE_HEREDOC)
			{
				heredoc_pid = heredoc_open_fd_pipe(data->current, &token);
				waitpid(heredoc_pid, NULL, 0);
				flag = 1;
				break ;
			}
			token = token->next;
		}
		if (flag == 1)
			break ;
		data->current = data->current->next;
	}
}

static void	handle_execve_processes(t_process_data *data, t_env *env)
{
	while (data->current)
	{
		if (pipe(data->current->fd) == -1)
			exit_with_error("pipe", data->child_pids);
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
	init_execve(current, &(data.child_pids));
	if (data.current->nb_pipes != 0)
		handle_heredoc_tokens(&data);
	data.current = current;
	handle_execve_processes(&data, env);
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

void	wait_exec(t_main *data)
{
	int	i;
	int	status;

	i = 0;
	signal(SIGINT, SIG_IGN);
	waitpid(data->pid_last, &status, 0);
	while (i < data->pipe_count)
	{
		waitpid(-1, NULL, 0);
		i++;
	}
	signal(SIGINT, sig_handler);
	if (WIFEXITED(status))
		data->return_value = WEXITSTATUS(status);
}
*/