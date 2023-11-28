/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/28 10:23:55 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_input(t_arg_handler *arg_handler, char *arg, int *arg_idx)
{
	int	*i;

	i = arg_handler->i;
	while (arg_handler->input[*i] && (*arg_handler->double_quote
			|| *arg_handler->single_quote || arg_handler->input[*i] != ' '))
	{
		handle_quotes_echo(arg_handler->input, arg_handler->i,
			arg_handler->double_quote, arg_handler->single_quote);

		if (is_redirection(arg_handler->input[*i])
			&& !*arg_handler->double_quote && !*arg_handler->single_quote)
			ft_skip_redirection_and_file(arg_handler->input, i);
		else if (arg_handler->input[*i] == '$' && !*arg_handler->single_quote)
			handle_arg_value(arg_handler);
		else
			arg[(*arg_idx)++] = arg_handler->input[(*i)++];
	}
	arg[*arg_idx] = '\0';
}

char	*ft_allocate_and_copy(t_arg_handler *arg_handler)
{
	char	*arg;
	int		*arg_idx;
	int		*i;
	int		size_of_argument;

	size_of_argument = calculate_size_of_argument(arg_handler->input);
	arg = malloc(sizeof(char) * (size_of_argument + 1));
	if (!arg)
		return (NULL);
	arg_handler->arg = arg;
	arg_idx = arg_handler->arg_idx;
	*arg_idx = 0;
	i = arg_handler->i;
	initialize_bools(arg_handler);
	process_input(arg_handler, arg, arg_idx);
	skip_spaces_echo(arg_handler->input, i);
	return (arg);
}

//////////////////////////////////////////////////////////////////////////////////////////////

// static char	*get_exit_status_str(void)
// {
// 	char	*str;

// 	str = ft_itoa(g_exit_status);
// 	if (!str)
// 		return (NULL);
// 	return (str);
// }

// static void	extract_variable_name(char *input, int *i, int *start, char **str)
// {
// 	int	end;

// 	end = *start;
// 	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
// 	{
// 		(*i)++;
// 	}
// 	end = *i - *start;
// 	*str = ft_substr(input, *start, end);
// 	if (!(*str))
// 		return ;
// }

// static void	append_env_value(t_parser *parser, char *env_value)
// {
// 	while (*env_value)
// 		parser->arg[parser->arg_idx++] = *env_value++;
// }

// static void	handle_dollar(t_parser *parser)
// {
// 	char	*str;
// 	char	*env_value;
// 	int		start;

// 	start = parser->i + 1;
// 	if (parser->input[start] == '?')
// 	{
// 		str = get_exit_status_str();
// 		append_env_value(parser, str);
// 		free(str);
// 		parser->i += 2;
// 	}
// 	else
// 	{
// 		(parser->i)++;
// 		extract_variable_name(parser->input, &(parser->i), &start, &str);
// 		env_value = get_env_value(parser->env, str);
// 		if (env_value)
// 		{
// 			append_env_value(parser, env_value);
// 		}
// 	}
// }

// static void	handle_arg_value_echo(t_parser *parser)
// {
// 	if (parser->input[parser->i] == '$'
// 		&& parser->input[parser->i + 1] != '$'
// 		&& (ft_isalpha(parser->input[parser->i + 1]) == 1
// 			|| parser->input[parser->i + 1] == '?'))
// 		handle_dollar(parser);
// 	else
// 		parser->arg[parser->arg_idx++]
// 			= parser->input[parser->i++];
// }


// void initialize_bools_echo(t_parser *parser)
// {
// 	parser->double_quote = false;
// 	parser->single_quote = false;
// }

// void	process_input_echo(t_parser *parser)
// {
// 	// printf("%sStart process_input_echo\n%s", CYAN, RESET);


//     // parser->arg = malloc(sizeof(char) * (SIZE + 1));


// 	while (parser->input[parser->i] && ((&(parser->double_quote))
// 			|| (&(parser->single_quote) || parser->input[parser->i] != ' ')))
// 	{
// 		handle_quotes_echo(parser->input, &(parser->i), &(parser->double_quote), &(parser->single_quote));
		
// 		// printf("%sparser->input[parser->i] = [%c]\n%s", CYAN, parser->input[parser->i], RESET);
		
// 		if (is_redirection(parser->input[parser->i]) && !&(parser->double_quote) && !&(parser->single_quote))
// 		{
// 			// printf("%s1\n%s", CYAN, RESET);
// 			ft_skip_redirection_and_file(parser->input, &(parser->i));
// 		}
// 		else if (parser->input[parser->i] == '$' && !&(parser->single_quote))
// 		{
// 			// printf("%s2\n%s", CYAN, RESET);
// 			handle_arg_value_echo(parser);
// 		}
// 		else
// 		{
// 			// printf("%sparser->arg[parser->arg_idx++] = [%d]\n%s", CYAN, parser->arg[parser->arg_idx++], RESET);
// 			parser->arg[parser->arg_idx++] = parser->input[parser->i++];
// 			// printf("%sje copie :\n%s", CYAN, RESET);
// 			// printf("%s	parser->input[parser->i++] = [%d]\n%s", CYAN, parser->input[parser->i++], RESET);
// 		}
// 	}
// 	parser->arg[parser->arg_idx] = '\0';
// 	// printf("%sparser->arg[parser->arg_idx] = [%d]\n%s", CYAN, parser->arg[parser->arg_idx], RESET);
// 	// printf("%sEND process_input_echo\n%s", CYAN, RESET);
// }

// char	*ft_allocate_and_copy(t_parser *parser)
// {
// 	int		start;
// 	int		len;
// 	char	*arg;
// 	int		j;
	
// 	start = parser->i;
// 	len = 0;
// 	while (parser->input[parser->i] && parser->input[parser->i] != ' ')
// 	{
// 		len++;
// 		parser->i++;
// 	}
// 	if (!(arg = malloc(sizeof(char) * (len + 1))))
// 		return NULL;
// 	j = 0;
// 	while (j < len)
// 	{
// 		arg[j] = parser->input[start + j];
// 		j++;
// 	}
// 	arg[len] = '\0';

// 	initialize_bools_echo(parser);
// 	// process_input_echo(parser); // coms marche le basic
// 	skip_spaces_echo(parser->input, &(parser->i));
// 	return (arg);
// }

