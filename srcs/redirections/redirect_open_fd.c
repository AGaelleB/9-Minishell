/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:07:58 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/05 11:32:16 by abonnefo         ###   ########.fr       */
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

int	open_fd(t_process_data *data, t_env *env, t_command *command)
{
	t_token	*token;
	t_token	*token_head;

	token = command->token_head;
	token_head = command->token_head;
	set_last_redirection_flag(data->command, token_head);
	while (token)
	{
		heredoc_open_fd(data, command, &token);
		redirect_file_in_open_fd(data, env, token);
		redirect_file_out_open_fd(data, env, token);
		redirect_append_file_out_open_fd(data, env, token);
		token = token->next;
	}
	return (0);
}
