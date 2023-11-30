/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:49:16 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/30 14:04:21 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	skip_spaces_echo(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

char	**copy_argument_echo(t_env *env, t_parser *parser, char *input)
{
	t_arg_handler	arg_handler;
	char			*arg;
	int				arg_idx;

	arg_idx = 0;
	arg_handler.env = env;
	arg_handler.input = input;
	arg_handler.i = &(parser->i);
	arg_handler.arg_idx = &arg_idx;
	arg_handler.arg = NULL;
	arg = ft_allocate_and_copy(&arg_handler);
	if (!arg)
		return (NULL);
	if (arg_idx > 0)
		parser->args[(parser->idx)++] = arg;
	else
		free(arg);
	skip_spaces_echo(input, &(parser->i));
	return (parser->args);
}

char	**parse_input_quote_echo(t_env *env, char *input)
{
	t_parser	parser;
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input);
	parser.args = malloc((arg_count + 1) * sizeof(char *));
	if (!parser.args)
		return (NULL);
	parser.in_quote = false;
	parser.idx = 0;
	while (input[parser.i]) //
	{
		parser.args = copy_argument_echo(env, &parser, input); //
		if (!parser.args)
			return (NULL);
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}
