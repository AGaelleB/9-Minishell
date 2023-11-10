/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:37:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/10 10:42:10 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_process_data *data, t_env *env)
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
	ft_builtin_write_exit(data->current->command);
	if (builtins_verif(data->current, env) == 1)
	{
		ft_close_all_fd();
		ft_free_all(data->current, data->current->token_head);
		exit (g_exit_status);
	}
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
	{
		handle_child_process(data, env);
		clean_heredoc_files(data->current);
	}
	else if (data->pid > 0)
		handle_parent_process(data);
	else
		exit_with_error("fork", data->child_pids);
}
