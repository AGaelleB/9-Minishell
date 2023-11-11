/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_out.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 17:27:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/11 14:43:26 by bfresque         ###   ########.fr       */
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

// int	heredoc_open_fd_pipe(t_command *command, t_token **token)
// {
// 	// printf("heredoc_open_fd_pipe\n");
// 	if (*token && (*token)->type == TYPE_HEREDOC)
// 	{
// 		handle_multiple_heredocs(command, *token);
// 		if (command->fd_in != -1)
// 		{
// 			dup2(command->fd_in, 0);
// 			close(command->fd_in);
// 		}
// 	}
// 	return (0);
// }

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
