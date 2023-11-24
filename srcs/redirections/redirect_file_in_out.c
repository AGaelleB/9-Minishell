/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_out.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/24 16:47:37 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_open_fd(t_process_data *data,
	t_command *command, t_token **token)
{
	if (*token && (*token)->type == TYPE_HEREDOC)
	{
		command->fd_in = data->heredocs[data->current_hd].fd[0];
		data->current_hd++;
		if (command->fd_in != -1)
		{
			dup2(command->fd_in, 0);
			close(command->fd_in);
		}
		// free(data->heredocs);
	}
}

void	redirect_file_in_open_fd(t_process_data *data, t_env *env, t_token *token)
{
	if (token->type == TYPE_REDIR_IN
		&& data->command->last_redir_is_heredoc == false)
	{
		if (redirect_file_in(data, env, token) == 0)
		{
			dup2(data->command->fd_in, 0);
			close(data->command->fd_in);
		}
	}
}

void	redirect_file_out_open_fd(t_process_data *data, t_env *env, t_token *token)
{
	if (token->type == TYPE_REDIR_OUT)
	{
		if (redirect_file_out(data, env, token) == 0)
		{
			dup2(data->command->fd_out, 1);
			close(data->command->fd_out);
		}
	}
}

void	redirect_append_file_out_open_fd(t_process_data *data, t_env *env, t_token *token)
{
	if (token->type == TYPE_REDIR_APPEND)
	{
		if (redirect_append_file_out(data, env, token) == 0)
		{
			dup2(data->command->fd_out, 1);
			close(data->command->fd_out);
		}
	}
}
