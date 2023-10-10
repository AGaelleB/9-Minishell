/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args_quotes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 17:01:04 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/10 15:34:21 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_args_single_quotes(char *input)
{
	int		count;
	bool	in_quote;
	bool	was_space;

	count = 0;
	in_quote = false;
	was_space = true;
	while (*input)
	{
		if (*input == '\'')
			in_quote = !in_quote;
		if (!in_quote && *input == ' ')
			was_space = true;
		else if (was_space)
		{
			count++;
			was_space = false;
		}
		input++;
	}
	return (count);
}

int	count_args_double_quotes_args(char *input)
{
	int		count;
	bool	in_quote;
	bool	was_space;

	count = 0;
	in_quote = false;
	was_space = true;
	while (*input)
	{
		if (*input == '\"')
			in_quote = !in_quote;
		if (!in_quote && *input == ' ')
			was_space = true;
		else if (was_space)
		{
			count++;
			was_space = false;
		}
		input++;
	}
	return (count);
}
