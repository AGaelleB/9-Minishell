/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:49:16 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/27 17:22:48 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	skip_spaces_echo(char *input, int *i)
// {
// 	while (input[*i] == ' ')
// 		(*i)++;
// }

// char	**copy_argument_echo(t_env *env, t_parser *parser, char *input)
// {
// 	t_arg_handler	arg_handler;
// 	char			*arg;
// 	int				arg_idx;

// 	arg_idx = 0;
// 	arg_handler.env = env;
// 	arg_handler.input = input;
// 	// arg_handler.i = &(parser->i);
// 	arg_handler.i = &(parser->i);
// 	arg_handler.arg_idx = &arg_idx;
// 	arg_handler.arg = NULL;
// 	arg = ft_allocate_and_copy(&arg_handler);
// 	if (!arg)
// 		return (NULL);
// 	if (arg_idx > 0)
// 		parser->args[(parser->idx)++] = arg;
// 	else
// 		free(arg);
// 	skip_spaces_echo(input, &(parser->i));
// 	return (parser->args);
// }

// char	**parse_input_quote_echo(t_env *env, char *input)
// {
// 	t_parser	parser;
// 	int			arg_count;

// 	parser.i = is_redir_at_beginning(input, 0);
// 	arg_count = count_args_single_quotes(input);
// 	parser.args = malloc((arg_count + 1) * sizeof(char *));
// 	if (!parser.args)
// 		return (NULL);
// 	parser.in_quote = false;
// 	parser.idx = 0;
// 	while (input[parser.i])
// 	{
// 		parser.args = copy_argument_echo(env, &parser, input);
// 		if (!parser.args)
// 			return (NULL);
// 	}
// 	parser.args[parser.idx] = NULL;
// 	return (parser.args);
// }

///////////////////////////////////////////////////////////////////////////////////////


void	skip_spaces_echo(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

char **copy_argument_echo(t_parser *parser, char *input)
{
	char *arg;

	parser->arg = NULL;
	arg = ft_allocate_and_copy(parser); // Modified to accept t_parser
	if (!arg)
		return (NULL);
	// Add the argument to the parser->args array
	parser->args[parser->idx++] = arg;
	// printf("%sje copie arg: '%s'\n%s", YELLOW, arg, RESET);

	// Skip spaces to find the start of the next argument
	skip_spaces_echo(input, &(parser->i));

	return (parser->args);
}

char **parse_input_quote_echo(t_env *env, char *input)
{
	t_parser	parser;
	// int			arg_count;
	int			j;

	parser.i = is_redir_at_beginning(input, 0);
	parser.in_quote = false;
	parser.idx = 0;
	parser.env = env;
	parser.input = input;
	parser.arg_idx = 0;
	parser.double_quote = false;
	parser.single_quote = false;
	j = parser.i;

	// arg_count = count_args_single_quotes(input);
	// parser.args = malloc((arg_count + 1) * sizeof(char *));
	parser.args = malloc(sizeof(char *) * SIZE);
	if (!parser.args)
		return (NULL);
		
	// printf("%sDebug: parse_input_quote_echo: '%s'\n%s",GREEN, input, RESET);

	while (input[j])
	{
		// handle_quotes_echo(parser.input, &(parser.i), &(parser.double_quote), &(parser.single_quote));
		
		// process_input_echo(&parser); 
		// printf("%s*** input[parser.i] = %d ***%s\n", RED, input[parser.i], RESET);
		
		if (!copy_argument_echo(&parser, input)) // Changed to pass t_parser
			return (NULL);
		// printf("%sDebug: Parsed arg: '%s'\n%s", GREEN, parser.args[parser.idx - 1], RESET);
		j++;
	}
	parser.args[parser.idx] = NULL;
	
	// for (int j = 0; parser.args[j] != NULL; j++)
	// 	printf("%sDebug: parser->args[%d] = '%s'\n%s", GREEN, j, parser.args[j], RESET);

	return (parser.args);
}
