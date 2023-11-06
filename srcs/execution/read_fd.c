/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/03 12:38:27 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_status;

/* void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;
	int	status;

	// i = 0;
	i = -1;
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		++i;
		waitpid(child_pids[i], &status, 0);
		// i++;
	}
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	signal(SIGINT, ft_builtin_ctrl_c);
	
	// printf("%sexit_status = %d%s\n", MAGENTA, command->exit_status, RESET);
} */

// void	wait_for_children(t_command *command, pid_t *child_pids)
// {
// 	int i;
// 	// int status;

// 	i = -1;
// 	signal(SIGINT, SIG_IGN);
// 	while (++i <= command->nb_pipes)
// 	{
// 		if (waitpid(child_pids[i], &g_exit_status, 0) > 0)
// 		{
// 			if (WIFEXITED(g_exit_status))
// 				g_exit_status = WEXITSTATUS(g_exit_status);
// 			else if (WIFSIGNALED(g_exit_status))
// 				g_exit_status = WTERMSIG(g_exit_status) + 128;
// 		}
// 		printf("Debug: Child exited with status %d\n", g_exit_status);
// 	}
// 	signal(SIGINT, ft_builtin_ctrl_c);
// }


void wait_for_children(t_command *command, pid_t *child_pids)
{
	int i;
	int status; // Use a local status variable

	i = -1;
	signal(SIGINT, SIG_IGN);
	while (++i <= command->nb_pipes)
	{
		if (waitpid(child_pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status); // Update global exit status
			else if (WIFSIGNALED(status))
				g_exit_status = WTERMSIG(status) + 128; // Update global exit status for signal termination
		}
		// printf("%swait_for_children: Child exited with status %d%s\n", RED, g_exit_status, RESET);
	}
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

/* // RAYAN
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