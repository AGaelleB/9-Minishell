/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:28:21 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/10 11:56:45 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	skip_space(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
	return (*i);
}

static void	process_single_argument(t_arg_handler *arg_handler, int *i)
{
	bool	*double_quote;
	bool	*single_quote;

	double_quote = arg_handler->double_quote;
	single_quote = arg_handler->single_quote;
	while (arg_handler->input[*i] && (*double_quote
			|| *single_quote || arg_handler->input[*i] != ' '))
	{
		handle_quotes_echo(arg_handler);
		(*i)++;
	}
}

int	count_args(char *input, int i)
{
	t_arg_handler	arg_handler;
	int				arg_count;

	arg_count = 0;
	arg_handler.input = input;
	arg_handler.i = &i;
	initialize_bools(&arg_handler);
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
		process_single_argument(&arg_handler, &i);
	}
	return (arg_count);
}

static int	process_arg_length(t_arg_handler *arg_handler, int *i)
{
	int		length;
	bool	*double_quote;
	bool	*single_quote;

	length = 0;
	double_quote = arg_handler->double_quote;
	single_quote = arg_handler->single_quote;
	while (arg_handler->input[*i] && (*double_quote || *single_quote
			|| arg_handler->input[*i] != ' '))
	{
		length++;
		handle_quotes_echo(arg_handler);
		(*i)++;
	}
	return (length);
}

int	count_arg_length(char *input, int i)
{
	t_arg_handler	arg_handler;
	int				length;

	length = 0;
	arg_handler.input = input;
	arg_handler.i = &i;
	initialize_bools(&arg_handler);
	while (input[i])
	{
		if (is_redirection(input[i]))
		{
			ft_skip_redirection_and_file(input, &i);
			continue ;
		}
		length += process_arg_length(&arg_handler, &i);
		while (input[i] == ' ')
			i++;
	}
	return (length);
}
