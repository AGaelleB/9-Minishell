/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/27 17:15:50 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_process_data *data, t_env *env)
{
	// printf("handle_child_process \n");
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

// void	handle_all_process(t_process_data *data, t_env *env)
// {
// 	// printf("handle_all_process \n");
// 	if (data->pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL); // coucou vol
// 		signal(SIGQUIT, SIG_DFL); // coucou vol

// 		handle_child_process(data, env);
// 	}
// 	else if (data->pid > 0)
// 		handle_parent_process(data);
// 	else
// 		exit_with_error("fork", data->child_pids);
// }

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;

	i = -1;
	// signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		
		++i;
		// signal(SIGINT, signal_cmd_2);
		// signal(SIGQUIT, SIG_IGN);
		waitpid(child_pids[i], NULL, 0);
		// signal(SIGINT, signal_cmd);
		// signal(SIGQUIT, SIG_IGN);
	}
	// signal(SIGINT, ft_builtin_ctrl_c);
	// signal(SIGINT, sighandler_heredoc);
}



void	signal_cmd_2(int sig)
{
	// g_exit_status += sig;
	if (sig == 2)
	{
		// g_exit_status = 130;
		printf("\n");
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_cmd(int sig)
{
	// g_exit_status += sig;
	if (sig == 2)
	{
		// g_exit_status = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
		exit (1);
	}
}

void	execve_fd(t_command *current, t_env *env)
{
	t_process_data    data;
	t_token *token;

	data.command = current;
	data.current = current;
	token = data.current->token_head;
	data.infile = 0;
	data.index = 0;
	init_execve(current, &(data.child_pids));
	while (token)
	{
		if (token->type == TYPE_HEREDOC)
			heredoc_open_fd(current, &token);
		else
			token = token->next;
	}
	while (data.current)
	{
		if (pipe(data.current->fd) == -1)
			exit_with_error("pipe", data.child_pids);
		signal(SIGINT, SIG_IGN); // coucou vol
		signal(SIGQUIT, SIG_IGN); // coucou vol
		data.pid = fork();
		data.child_pids[data.index++] = data.pid;
		data.current->fd_in = data.current->fd[0];
		data.current->fd_out = data.current->fd[1];

		if (data.pid == 0)
		{
			signal(SIGINT, SIG_DFL); // coucou vol
			signal(SIGQUIT, SIG_DFL); // coucou vol

			handle_child_process(&data, env);
		}
		else if (data.pid > 0)
			handle_parent_process(&data);
		else
			exit_with_error("fork", data.child_pids);

		
		// handle_all_process(&data, env);
		data.current = data.current->next;
	}
	// handle_signals_heredoc(); // 
	// wait_for_children(data.command, data.child_pids);
	// handle_signals_heredoc(); // 

	signal(SIGINT, signal_cmd_2); // coucou vol
	signal(SIGQUIT, SIG_IGN); // coucou vol
	wait_for_children(data.command, data.child_pids);
	signal(SIGINT, signal_cmd); // coucou vol
	signal(SIGQUIT, SIG_IGN); // coucou vol
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