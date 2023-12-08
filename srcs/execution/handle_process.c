/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:37:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/08 13:23:13 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_child(t_process_data *data, t_env *env)
{
	if (data->count_hd)
		free(data->heredocs);
	ft_free_env(env);
	ft_free_all(data->command, data->command->token_head);
	cleanup(data->child_pids, data->infile);
}

static void	setup_input_output(t_process_data *data)
{
	close(data->current->fd_in);
	dup2(data->infile, 0);
	if (data->current->next)
		dup2(data->current->fd_out, 1);
	close(data->current->fd_out);
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
}

void	handle_child_process(t_process_data *data, t_env *env)
{
	setup_input_output(data);
	open_fd(data, env, data->current);
	dup2(data->current->fd_out, 1);
	close(data->current->fd_out);
	ft_close_fd();
	if (!data->current->next)
	{
		if (builtins_verif(data->current, env) == 1)
		{
			ft_builtin_write_exit_process(data, env);
			ft_free_tab(data->command->command_arg_main);
			free_child(data, env);
			exit(g_exit_status);
		}
		else
			execve_process(data, env);
	}
	else
		execve_process(data, env);
	free_child(data, env);
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
	{
		handle_parent_process(data);
		if (data->count_hd)
			free(data->heredocs);
	}
	else
		exit_with_error("fork", data->child_pids);
}
