/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:49:16 by bfresque          #+#    #+#             */
/*   Updated: 2023/10/13 11:06:25 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* extention of ft_allocate_and_copy*/
void	handle_quotes_echo(char *input, int *i,
	bool *double_quote, bool *single_quote)
{
	if (input[*i] == '\"' && !*single_quote)
	{
		*double_quote = !*double_quote;
		(*i)++;
	}
	else if (input[*i] == '\'' && !*double_quote)
	{
		*single_quote = !*single_quote;
		(*i)++;
	}
}

/*Fonction 1: Gestion des espaces après un argument*/
void	skip_spaces_echo(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

/*Fonction 3: Copie d'un argument dans le tableau d'arguments*/
char	**copy_argument_echo(char *input, t_parser *parser)
{
	char	*arg;
	int		arg_idx;

	arg = ft_allocate_and_copy(input, &(parser->i), &arg_idx);
	if (!arg)
		return (NULL);
	if (arg_idx > 0)
		parser->args[(parser->idx)++] = arg;
	else
		free(arg);
	skip_spaces_echo(input, &(parser->i));
	return (parser->args);
}

/*Fonction 4 (principale): Parse Input avec Quotes*/
char	**parse_input_quote_echo(char *input)
{
	t_parser	parser;
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input);
	parser.args = malloc((arg_count + 1) * sizeof(char *));
	parser.in_quote = false;
	parser.idx = 0;
	if (!parser.args)
		return (NULL);
	while (input[parser.i])
	{
		parser.args = copy_argument_echo(input, &parser);
		if (!parser.args)
			return (NULL);
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}
