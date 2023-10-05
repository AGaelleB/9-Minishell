/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_and_out.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:06:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 17:07:34 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redirect_file_in(t_command *current, t_token *token) // <
{
	char	*filename;

	if (current->fd_in != 0)
		close(current->fd_in);
	filename = token->next->split_value;
	current->fd_in = open(filename, O_RDONLY);
	if (current->fd_in == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_file_out(t_command *current, t_token *token) // >
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = token->next->split_value; //
	// if (token->next == NULL)
	// 	filename = token->split_value;
	current->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_append_file_out(t_command *current, t_token *token) // >>
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = token->next->split_value;
	current->fd_out = open(filename, O_APPEND | O_WRONLY, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	is_redir_at_beginning(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	if ((input[i] == '>') || (input[i] == '<')
		|| (input[i] == '>' && input[i + 1] == '>')
		|| (input[i] == '<' && input[i + 1] == '<'))
	{
		if ((input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '<' && input[i + 1] == '<'))
			i += 2;
		else if (input[i] == '>' || input[i] == '<')
			i++;
		while (input[i] == ' ')
			i++;
		while (input[i] != ' ')
			i++;
		return (is_redir_at_beginning(input, i));
	}
	return (i);
}