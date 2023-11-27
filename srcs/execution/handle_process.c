/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:37:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/27 11:40:34 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_child(t_process_data *data, t_env *env)
{
	// ft_close_all_fd();

	
	if (data->count_hd)
	{
		// printf("%s data->count_hd = %d %s\n", RED, data->count_hd, RESET);
		free(data->heredocs);
	}
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
	open_fd(data, env, data->current);
	ft_close_fd();
	ft_builtin_write_exit_process(data, env);
	if (builtins_verif(data->current, env) == 1)
	{
		ft_free_tab(data->command->command_arg_main); // test, if ? 
		free_child(data, env);
		exit(g_exit_status);
	}
	execve_process(data, env);
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
