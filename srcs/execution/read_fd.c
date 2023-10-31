/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/31 17:45:41 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_process_data *data, t_env *env)
{
	close(data->current->fd_in); // a priori close le pipe du heredoc et ne permet plus de le lire
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
	if (builtins_verif(data->current, env) == 1)
		exit(0);
	// ft_set_args_and_paths(data->current, env);
	// print_commands_and_tokens(current); // PRINT
	if (execve_process(data->current, env) == 127)
		exit(127);
}

void	handle_parent_process(t_process_data *data)
{
	close(data->current->fd_out);
	if (data->infile != 0)
		close(data->infile);
	data->infile = data->current->fd_in;
}

void	handle_all_process(t_process_data *data, t_env *env)
{
	if (data->pid == 0)
		handle_child_process(data, env);
	else if (data->pid > 0)
		handle_parent_process(data);
	else
		exit_with_error("fork", data->child_pids);
}

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;
	int	status; // AJOUT
	// int	exit_status; // AJOUT
	// int	term_signal; // AJOUT

	// i = -1; 
	i = 0;
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		// ++i;
		waitpid(child_pids[i], &status, 0); // AJOUT
		i++;
	}
	// if (WIFEXITED(status))
	// 	exit_status = WEXITSTATUS(status);
	// if (WIFSIGNALED(status))
	// 	term_signal = WTERMSIG(status);
	signal(SIGINT, ft_builtin_ctrl_c);
}

void execve_fd(t_command *current, t_env *env)
{
	t_process_data data;
	t_token *token;
	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	token = current->token_head;
	init_execve(current, &(data.child_pids));

	int flag = 0;
	if (data.current->nb_pipes != 0)
	{
		while(data.current && flag == 0)
		{
			while(token && flag == 0)
			{
				if (token->type == TYPE_HEREDOC)
				{
					
					pid_t heredoc_pid = heredoc_open_fd_pipe(current, &token);
					waitpid(heredoc_pid, NULL, 0);
					flag = 1;
					break;
				}
				token = token->next;
			}
			if (flag == 1)
				break;
			
			data.current = data.current->next;
		}
	}
	// printf("1 - j'ai fini de wait le pid de heredoc\n");
	data.current = current;
	while (data.current)
	{
		if (pipe(data.current->fd) == -1)
			exit_with_error("pipe", data.child_pids);
		data.pid = fork();
		data.child_pids[data.index++] = data.pid;
		data.current->fd_in = data.current->fd[0];
		data.current->fd_out = data.current->fd[1];
		handle_all_process(&data, env);
		data.current = data.current->next;
	}
	// printf("2 - je vais wait le pid\n");
	waitpid(data.pid, NULL, 0); // omg
	// printf("3 - j'ai wait le pid\n");
	cleanup(data.child_pids, data.infile);
}

/* void	execve_fd(t_command *current, t_env *env)
{
	t_process_data	data;

	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	init_execve(current, &(data.child_pids));
	while (data.current)
	{
		if (pipe(data.current->fd) == -1)
			exit_with_error("pipe", data.child_pids);
		data.pid = fork();
		data.child_pids[data.index++] = data.pid;
		data.current->fd_in = data.current->fd[0];
		data.current->fd_out = data.current->fd[1];
		handle_all_process(&data, env);
		data.current = data.current->next;
	}
	wait_for_children(data.command, data.child_pids);
	cleanup(data.child_pids, data.infile);
}
 */


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