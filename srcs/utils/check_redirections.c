/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:53:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/02 14:03:44 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_redirection(char c)
{
	return (c == '>' || c == '<');
}

void	ft_skip_filename(char *input, int *i, bool *single_quote,
	bool *double_quote)
{
	while (input[*i] && !is_redirection(input[*i])
		&& (*single_quote || *double_quote || input[*i] != ' '))
	{
		if (input[*i] == '\'')
			*single_quote = !(*single_quote);
		else if (input[*i] == '\"')
			*double_quote = !(*double_quote);
		if ((input[*i] == '\'' && !(*single_quote))
			|| (input[*i] == '\"' && !(*double_quote)))
			(*i)++;
		else
			(*i)++;
	}
}

void	ft_skip_redirection_and_file(char *input, int *i)
{
	bool	single_quote;
	bool	double_quote;

	single_quote = false;
	double_quote = false;
	if (!is_redirection(input[*i]))
		return ;
	(*i)++;
	skip_spaces_echo(input, i);
	ft_skip_filename(input, i, &single_quote, &double_quote);
	skip_spaces_echo(input, i);
}

int	check_bad_redir(char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		while (input[i] == '<' || input[i] == '>')
		{
			count++;
			i++;
		}
		if (count > 2)
			return (1);
		count = 0;
		i++;
	}
	return (0);
}
