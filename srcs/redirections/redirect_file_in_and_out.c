/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_and_out.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:06:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/11 17:25:29 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*verif_file_name(t_token *token, t_token *token_head)
{
	char	*file_name;

	file_name = NULL;
	if (token->next->split_value[0] == '\"'
		|| token->next->split_value[0] == '\'')
		file_name = epur_filename(token_head);
	else
	{
		file_name = token->next->split_value;
		if (check_valid_caractere_filename(file_name[0]) == 1)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			ft_putchar_fd(file_name[0], 2);
			ft_putstr_fd("\n", 2);
			exit(-1);
		}
	}
	return (file_name);
}

int	redirect_file_out(t_command *current, t_token *token, t_token *token_head)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = verif_file_name(token, token_head);
	current->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_file_in(t_command *current, t_token *token, t_token *token_head)
{
	char	*filename;

	if (current->fd_in != 0)
		close(current->fd_in);
	filename = verif_file_name(token, token_head);
	current->fd_in = open(filename, O_RDONLY);
	if (current->fd_in == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_append_file_out(t_command *current,
	t_token *token, t_token *token_head)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = verif_file_name(token, token_head);
	current->fd_out = open(filename, O_APPEND | O_WRONLY, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

/* int	is_redir_at_beginning(char *input, int i)
{
	bool	in_quote;

	in_quote = false;
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
		if(input[i] == '\'' || input[i] == '\"')
		{
			in_quote = !in_quote;
			i++;
		}
		while(in_quote)
		{
			if(input[i] == '\'' || input[i] == '\"')
				in_quote = !in_quote;
			i++;
		}
		while (input[i] != ' ')
			i++;
		return (is_redir_at_beginning(input, i));
	}
	return (i);
}
 */