/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:07:58 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/11 17:28:06 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			return ;
		}
		else if (last_token->type == TYPE_REDIR_IN)
		{
			command->last_redir_is_heredoc = false;
			return ;
		}
		last_token = last_token->prev;
	}
	command->last_redir_is_heredoc = false;
}

// static void	handle_heredoc_tokens(t_command *current, t_token *token)
// {
// 	// t_token	*token;
// 	pid_t	heredoc_pid;
// 	// int		flag;

// 	// token = current->token_head;
// 	// flag = 0;
// 	// while (current && flag == 0)
// 	// {
// 	// 	while (token && flag == 0)
// 	// 	{	
// 			if (token->type == TYPE_HEREDOC)
// 			{
// 				heredoc_pid = heredoc_open_fd_pipe(current, &token);
// 				waitpid(heredoc_pid, NULL, 0);
// 				// flag = 1;
// 				// break ;
// 			}
// 			// token = token->next;
// 		// }
// 		// if (flag == 1)
// 		// 	break ;
// 		// current = current->next;
// 	// }
// }

int	open_fd(t_process_data *data, t_command *command)
{
	t_token	*token;
	t_token	*token_head;


	// flag = 0;
	token = command->token_head;
	token_head = command->token_head;
	set_last_redirection_flag(command, token_head);
	// while (token)
	// {
	// 	if ((token->type == TYPE_HEREDOC))
	// 	{
	// 		heredoc_open_fd(command, &token);
	// 	}
	// 	else
	// 		token = token->next;
	// }
	// token = command->token_head;
	while (token)
	{
		heredoc_open_fd(data, command, &token);
		redirect_file_in_open_fd(command, token, token_head);
		redirect_file_out_open_fd(command, token, token_head);
		redirect_append_file_out_open_fd(command, token, token_head);
		token = token->next;
	}
	return (0);
}
