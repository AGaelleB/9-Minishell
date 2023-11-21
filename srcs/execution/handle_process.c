/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:37:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/21 13:48:55 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_child(t_process_data *data, t_env *env)
{
	// ft_close_all_fd();
	free(data->heredocs);
	ft_free_env(env);
	ft_free_all(data->current, data->current->token_head);
	cleanup(data->child_pids, data->infile); // on perd un test
}

void	handle_child_process(t_process_data *data, t_env *env)
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
	ft_close_fd();
	open_fd(data, data->current);
	ft_builtin_write_exit_process(data, env);
	if (builtins_verif(data->current, env) == 1)
	{
		free_child(data, env);
		exit(g_exit_status);
	}
	if (execve_process(data->current, env) == 127)
	{
		free_child(data, env);
		exit(127);
	}
	free_child(data, env);
}

void	handle_parent_process(t_process_data *data)
{
	close(data->current->fd_out);
	if (data->infile != 0)
		close(data->infile);
	data->infile = data->current->fd_in;
	// ft_free_tab(env->cpy_env); // fait perdre 25 tests
}

void	handle_all_process(t_process_data *data, t_env *env)
{
	if (data->pid == 0)
		handle_child_process(data, env);
	else if (data->pid > 0)
		handle_parent_process(data);
		// free(data->heredocs);  // on perd un test
	else
		exit_with_error("fork", data->child_pids);
}
