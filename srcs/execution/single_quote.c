/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 17:07:28 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Fonction 1: Allocation de l'argument et copie des caractères*/
char	*allocate_and_copy(char *input, int *i, bool *in_quote, int *arg_idx)
{
	char	*arg;

	arg = malloc(ft_strlen(input) + 1);
	if (!arg)
		return (NULL);
	*arg_idx = 0;
	while (input[*i] && (*in_quote
			|| (input[*i] != ' ' && input[*i] != '>' && input[*i] != '<')))
	{
		if (input[*i] == '\'')
			*in_quote = !*in_quote;
		else
			arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	return (arg);
}

/*Fonction 2: Gestion des espaces après un argument*/
void	skip_spaces(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

/*Fonction 3: Copie d'un argument dans le tableau d'arguments*/
char	**copy_argument(char *input, t_parser *parser)
{
	char	*arg;
	int		arg_idx;

	arg = allocate_and_copy(input, &(parser->i), &(parser->in_quote), &arg_idx);
	if (!arg)
		return (NULL);
	if (arg_idx > 0)
		parser->args[(parser->idx)++] = arg;
	else
		free(arg);
	skip_spaces(input, &(parser->i));
	return (parser->args);
}

/*Fonction 4 (principale): Parse Input avec Quotes*/
char	**parse_input_quote(char *input)
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
