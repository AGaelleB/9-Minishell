/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:07:58 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/25 13:06:11 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	heredoc_open_fd(t_command *command, t_token **token)
{
	if (*token && (*token)->type == TYPE_HEREDOC)
	{
		*token = handle_multiple_heredocs(command, *token);
		if (command->fd_in != -1)
		{
			dup2(command->fd_in, 0);
			close(command->fd_in);
		}
	}
}

void	redirect_file_in_open_fd(t_command *command,
	t_token *token, t_token *token_head)
{
	if (token->type == TYPE_REDIR_IN
		&& command->last_redir_is_heredoc == false)
	{
		if (redirect_file_in(command, token, token_head) == 0)
		{
			dup2(command->fd_in, 0);
			close(command->fd_in);
		}
	}
}

void	redirect_file_out_open_fd(t_command *command,
	t_token *token, t_token *token_head)
{
	if (token->type == TYPE_REDIR_OUT)
	{
		if (redirect_file_out(command, token, token_head) == 0)
		{
			dup2(command->fd_out, 1);
			close(command->fd_out);
		}
	}
}

void	redirect_append_file_out_open_fd(t_command *command,
	t_token *token, t_token *token_head)
{
	if (token->type == TYPE_REDIR_APPEND)
	{
		if (redirect_append_file_out(command, token, token_head) == 0)
		{
			dup2(command->fd_out, 1);
			close(command->fd_out);
		}
	}
}

void	set_last_redirection_flag(t_command *command, t_token *token_head)
{
	t_token	*token;
	t_token	*last_token;

	token = token_head;
	last_token = NULL;
	while (token)
	{
		last_token = token;
		token = token->next;
	}
	while (last_token)
	{
		if (last_token->type == TYPE_HEREDOC)
		{
			command->last_redir_is_heredoc = true;
			return;
		}
		else if (last_token->type == TYPE_REDIR_IN)
		{
			command->last_redir_is_heredoc = false;
			return;
		}
		last_token = last_token->prev;
	}
	command->last_redir_is_heredoc = false;
}

int	open_fd(t_command *command)
{
	t_token	*token;
	t_token	*token_head;

	token = command->token_head;
	token_head = command->token_head;
	set_last_redirection_flag(command, token_head);
	while (token)
	{
		if (token->type == TYPE_HEREDOC)
			heredoc_open_fd(command, &token);
		else
			token = token->next;
	}
	token = command->token_head;
	while (token)
	{
		redirect_file_in_open_fd(command, token, token_head);
		redirect_file_out_open_fd(command, token, token_head);
		redirect_append_file_out_open_fd(command, token, token_head);
		token = token->next;
	}
	return (0);
}
