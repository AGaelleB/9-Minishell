/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/07 16:50:27 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int redirect_file_in(t_command *cmd, t_token *token)
{
	if (token->type == TYPE_REDIR_IN)
	{
		cmd->input_fd = open(token->next->split_value, O_RDONLY);
		if (cmd->input_fd == -1)
		{
			perror("Error opening file for input redirection");
			return (-1);
		}
	}
	else if (token->type == TYPE_SEPARATOR)
	{
		// La gestion des pipes sera gérée dans la fonction `multiple_pipe`.
	}
	return (0);
}

int redirect_file_out(t_command *cmd, t_token *token)
{
	if (token->type == TYPE_REDIR_OUT)
	{
		cmd->output_fd = open(token->next->split_value, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (cmd->output_fd == -1)
		{
			perror("Error opening file for output redirection");
			return (-1);
		}
	}
	else if (token->type == TYPE_REDIR_APPEND)
	{
		cmd->output_fd = open(token->next->split_value, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (cmd->output_fd == -1)
		{
			perror("Error opening file for output appending");
			return (-1);
		}
	}
	else if (token->type == TYPE_SEPARATOR)
	{
		// La gestion des pipes sera gérée dans la fonction `multiple_pipe`.
	}
	return (0);
}

int open_fd(t_command *current, char **envp)
{
	t_token *token;

	while (current)
	{
		token = current->token;
		while (token)
		{
			if (token->type == TYPE_REDIR_IN)
			{
				redirect_file_in(current, token);
				token = token->next;  // pour passer le nom du fichier
			}
			else if (token->type == TYPE_REDIR_OUT || token->type == TYPE_REDIR_APPEND)
			{
				redirect_file_out(current, token);
				token = token->next;  // pour passer le nom du fichier
			}
			token = token->next;
		}
		multiple_pipe(current, envp, current->input_fd, current->output_fd);
		current = current->next;
	}
	return (0);
}

