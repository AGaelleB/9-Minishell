/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:07:58 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/24 11:54:04 by abonnefo         ###   ########.fr       */
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
		// printf("heredoc_open_fd  = %s\n", command->heredoc);
	}
}

void	redirect_file_in_open_fd(t_command *command,
	t_token *token, t_token *token_head)
{
	if (token->type == TYPE_REDIR_IN)
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

int	open_fd(t_command *command)
{
	t_token	*token;
	t_token	*token_head;

	token = command->token_head;
	token_head = command->token_head;
	while (token)
	{
		heredoc_open_fd(command, token);
		redirect_file_in_open_fd(command, token, token_head);
		redirect_file_out_open_fd(command, token, token_head);
		redirect_append_file_out_open_fd(command, token, token_head);
		token = token->next;
	}
	// printf("open_fd = %s\n", command->heredoc);
	return (0);
}
