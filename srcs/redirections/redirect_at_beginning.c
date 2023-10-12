/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_at_beginning.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 17:25:21 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/12 09:37:51 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	skip_spaces_redir_beginning(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

void	skip_redirection(char *input, int *i)
{
	if ((input[*i] == '>' && input[*i + 1] == '>')
		|| (input[*i] == '<' && input[*i + 1] == '<'))
		*i += 2;
	else if (input[*i] == '>' || input[*i] == '<')
		(*i)++;
}

void	skip_until_space_or_end(char *input, int *i)
{
	while (input[*i] != ' ' && input[*i] != '\0')
		(*i)++;
}

void	skip_quotes(char *input, int *i)
{
	bool	in_quote;

	in_quote = false;
	if (input[*i] == '\'' || input[*i] == '\"')
	{
		in_quote = !in_quote;
		(*i)++;
	}
	while (in_quote && input[*i] != '\0')
	{
		if (input[*i] == '\'' || input[*i] == '\"')
			in_quote = !in_quote;
		(*i)++;
	}
}

int	is_redir_at_beginning(char *input, int i)
{
	while (1)
	{
		skip_spaces_redir_beginning(input, &i);
		if ((input[i] == '>') || (input[i] == '<')
			|| (input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '<' && input[i + 1] == '<'))
		{
			skip_redirection(input, &i);
			skip_spaces_redir_beginning(input, &i);
			skip_quotes(input, &i);
			skip_until_space_or_end(input, &i);
		}
		else
			break ;
	}
	return (i);
}
