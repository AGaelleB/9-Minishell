/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 10:59:32 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 16:26:53 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes_echo(char *str, int *i, bool *double_quote,
	bool *single_quote)
{
	if (str[*i] == '\'' && !*double_quote && str[*i + 1] != '\'')
	{
		*single_quote = !*single_quote;
		(*i)++;
	}
	else if (str[*i] == '\"' && !*single_quote && str[*i + 1] != '\"')
	{
		*double_quote = !*double_quote;
		(*i)++;
	}
	while (!*single_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		*i += 2;
	while (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
		*i += 2;
	if (!*double_quote && str[*i] == '\'')
		*single_quote = !*single_quote;
	else if (!*single_quote && str[*i] == '\"')
		*double_quote = !*double_quote;
	if (!*single_quote && str[*i] == '\"')
		(*i)++;
	else if (!*double_quote && str[*i] == '\'')
		(*i)++;
}

static void	update_quote_flags(char c, bool *double_quote, bool *single_quote)
{
	if (c == '\"' && !*single_quote)
		*double_quote = !*double_quote;
	else if (c == '\'' && !*double_quote)
		*single_quote = !*single_quote;
}

int	calculate_size_of_argument(char *input)
{
	int		length;
	int		i;
	bool	double_quote;
	bool	single_quote;

	length = 0;
	i = 0;
	double_quote = false;
	single_quote = false;
	while (input[i++])
	{
		update_quote_flags(input[i], &double_quote, &single_quote);
		if ((double_quote || single_quote || input[i] != ' '))
		{
			if (input[i] == '\\' && input[i + 1] && (double_quote
					|| single_quote))
			{
				i++;
				length++;
			}
			else
				length++;
		}
	}
	return (length);
}

void	initialize_bools(t_arg_handler *arg_handler)
{
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg_handler->double_quote = &double_quote;
	arg_handler->single_quote = &single_quote;
}
