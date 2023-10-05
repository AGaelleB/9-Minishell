/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:07:58 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/02 16:33:12 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_open_fd(t_command *command, t_token *token)
{
	if (token->type == TYPE_HEREDOC)
	{
		if (redirect_heredoc(command, token) == 0)
		{
			dup2(command->fd_in, 0);
			close(command->fd_in);
		}
	}
}

void	redirect_file_in_open_fd(t_command *command, t_token *token)
{
	if (token->type == TYPE_REDIR_IN)
	{
		if (redirect_file_in(command, token) == 0)
		{
			dup2(command->fd_in, 0);
			close(command->fd_in);
		}
	}
}

void	redirect_file_out_open_fd(t_command *command, t_token *token)
{
	if (token->type == TYPE_REDIR_OUT)
	{
		if (redirect_file_out(command, token) == 0)
		{
			dup2(command->fd_out, 1);
			close(command->fd_out);
		}
	}
}

void	redirect_append_file_out_open_fd(t_command *command, t_token *token)
{
	if (token->type == TYPE_REDIR_APPEND)
	{
		if (redirect_append_file_out(command, token) == 0)
		{
			dup2(command->fd_out, 1);
			close(command->fd_out);
		}
	}
}

int	open_fd(t_command *command)
{
	t_token	*token;

	token = command->token_head;
	while (token)
	{
		heredoc_open_fd(command, token);
		redirect_file_in_open_fd(command, token);
		redirect_file_out_open_fd(command, token);
		redirect_append_file_out_open_fd(command, token);
		token = token->next;
	}
	return (0);
}