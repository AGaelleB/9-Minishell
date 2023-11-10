/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:28:21 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/10 10:36:09 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	skip_space(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
	return (*i);
}

int count_args(char *input, int i)
{
	t_arg_handler arg_handler;
	int arg_count = 0;
	bool double_quote = false;
	bool single_quote = false;

	arg_handler.input = input;
	arg_handler.i = &i;

	while (input[i])
	{
		i = skip_space(input, &i);
		if (!input[i])
			break;
		if (is_redirection(input[i]))
		{
			ft_skip_redirection_and_file(input, &i);
			continue;
		}
		arg_count++;
		while (input[i] && (double_quote || single_quote || input[i] != ' '))
		{
			arg_handler.double_quote = &double_quote;
			arg_handler.single_quote = &single_quote;
			handle_quotes_echo(&arg_handler);
			i++;
		}
	}
	return (arg_count);
}

int count_arg_length(char *input, int i)
{
	t_arg_handler arg_handler;
	int length = 0;
	bool double_quote = false;
	bool single_quote = false;

	arg_handler.input = input;
	arg_handler.i = &i;

	while (input[i])
	{
		if (is_redirection(input[i]))
		{
			ft_skip_redirection_and_file(input, &i);
			continue;
		}
		while (input[i] && (double_quote || single_quote || input[i] != ' '))
		{
			length++;
			arg_handler.double_quote = &double_quote;
			arg_handler.single_quote = &single_quote;
			handle_quotes_echo(&arg_handler);
			i++;
		}
		while (input[i] == ' ')
			i++;
	}
	return (length);
}
