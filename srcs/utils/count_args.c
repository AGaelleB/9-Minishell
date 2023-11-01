/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:28:21 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/01 15:49:06 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	skip_space(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
	return (*i);
}

int	count_args(char *input, int i)
{
	int		arg_count;
	bool	double_quote;
	bool	single_quote;

	arg_count = 0;
	double_quote = false;
	single_quote = false;
	while (input[i])
	{
		i = skip_space(input, &i);
		if (!input[i])
			break ;
		if (is_redirection(input[i]))
		{
			ft_skip_redirection_and_file(input, &i);
			continue ;
		}
		arg_count++;
		while (input[i] && (double_quote || single_quote || input[i] != ' '))
		{
			handle_quotes_echo(input, &i, &double_quote, &single_quote);
			i++;
		}
	}
	return (arg_count);
}

int	count_arg_length(char *input, int i)
{
	int		length;
	bool	double_quote;
	bool	single_quote;

	length = 0;
	double_quote = false;
	single_quote = false;
	while (input[i])
	{
		if (is_redirection(input[i]))
		{
			ft_skip_redirection_and_file(input, &i);
			continue ;
		}
		while (input[i] && (double_quote || single_quote || input[i] != ' '))
		{
			length++;
			handle_quotes_echo(input, &i, &double_quote, &single_quote);
			i++;
		}
		while (input[i] == ' ')
			i++;
	}
	return (length);
}
