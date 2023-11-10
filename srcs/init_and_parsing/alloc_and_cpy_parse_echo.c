/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/10 10:44:32 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes_echo(t_arg_handler *arg_handler)
{
	char	*str;
	int		*i;
	bool	*double_quote;
	bool	*single_quote;

	str = arg_handler->input;
	i = arg_handler->i;
	double_quote = arg_handler->double_quote;
	single_quote = arg_handler->single_quote;
	if (!*single_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		*i += 2;
	else if (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
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
	while (input[i] && (double_quote || single_quote || input[i] != ' '))
	{
		if (input[i] == '\"' && !single_quote)
		{
			double_quote = !double_quote;
			i++;
		}
		else if (input[i] == '\'' && !double_quote)
		{
			single_quote = !single_quote;
			i++;
		}
		else if (input[i] == '\\' && input[i + 1]
			&& (double_quote || single_quote))
		{
			i += 2;
			length++;
		}
		else
		{
			i++;
			length++;
		}
	}
	return (length);
}

char	*ft_allocate_and_copy(t_arg_handler *arg_handler)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;
	int		size_of_argument;
	int		*arg_idx;
	int		*i;

	size_of_argument = calculate_size_of_argument(arg_handler->input);
	double_quote = false;
	single_quote = false;
	arg = malloc(sizeof(char) * (size_of_argument + 1));
	if (!arg)
		exit(EXIT_FAILURE);
	arg_handler->arg = arg;
	arg_idx = arg_handler->arg_idx;
	*arg_idx = 0;
	i = arg_handler->i;
	arg_handler->double_quote = &double_quote;
	arg_handler->single_quote = &single_quote;
	while (arg_handler->input[*i] && (double_quote || single_quote
			|| arg_handler->input[*i] != ' '))
	{
		handle_quotes_echo(arg_handler);
		if (is_redirection(arg_handler->input[*i]) && !double_quote && !single_quote)
			ft_skip_redirection_and_file(arg_handler->input, i);
		else if (arg_handler->input[*i] == '$' && !single_quote)
			handle_arg_value(arg_handler);
		else
			arg[(*arg_idx)++] = arg_handler->input[(*i)++];
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(arg_handler->input, i);
	return (arg);
}
