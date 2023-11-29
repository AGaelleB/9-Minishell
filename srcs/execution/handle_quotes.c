/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/29 11:58:49 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes(char *input, int *i,
	bool *double_quote, bool *single_quote)
{
	char	quote_char;
	int		count;

	quote_char = input[*i];
	count = 0;
	if ((quote_char == '\"' || quote_char == '\'')
		&& !(*double_quote && *single_quote))
	{
		while (input[*i] == quote_char)
		{
			count++;
			(*i)++;
		}
		if (count % 2 != 0)
		{
			if (quote_char == '\"')
				*double_quote = !*double_quote;
			else
				*single_quote = !*single_quote;
		}
	}
}

char	*allocate_and_copy(char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(ft_strlen(input) + 1);
	if (!arg)
		return (NULL);
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes(input, i, &double_quote, &single_quote); // warning jump
		if ((input[*i] == ' ' || input[*i] == '>' || input[*i] == '<')
			&& !double_quote && !single_quote)
			break ;
		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	// arg[(*arg_idx)++] = '\0'; // test
	return (arg);
}

void	skip_spaces(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

char	**copy_argument(char *input, t_parser_one *parser)
{
	char	*arg;
	int		arg_idx;

	arg = allocate_and_copy(input, &(parser->i), &arg_idx);
	if (!arg)
		return (NULL);
	if (arg_idx > 0)
		parser->args[(parser->idx)++] = arg;
	else
		free(arg);
	skip_spaces(input, &(parser->i));
	// parser->args[(parser->idx)++] = '\0'; ///////////////////
	return (parser->args);
}

// t_parser	init_parser()
// {
// 	t_parser	parser;

// 	parser.args = NULL;
// 	parser.i = 0;
// 	parser.idx = 0;
// 	parser.in_quote = false;
	
// 	return (parser);
// }

char	**parse_input_quote(char *input)
{

	t_parser_one	parser;
	// t_parser	parser = init_parser();
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input);
	parser.args = malloc(sizeof(char *) * (arg_count + 1));
	if (!parser.args)
		return (NULL);
	parser.in_quote = false;
	parser.idx = 0;
	while (input[parser.i])
	{
		parser.args = copy_argument(input, &parser);
		if (!parser.args)
			return (NULL);
		if ((!parser.in_quote) && (input[parser.i] == '>'
				|| input[parser.i] == '<'))
			break ;
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}
