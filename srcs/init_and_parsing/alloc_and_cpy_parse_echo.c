/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/30 14:36:54 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	process_input(t_arg_handler *arg_handler, char *arg, int *arg_idx)
{
	int		*i;
	bool	*double_quote;
	bool	*single_quote;
	
	double_quote = arg_handler->double_quote;
	single_quote = arg_handler->single_quote;
	*double_quote = false;
	*single_quote = false;
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
	(void)size_of_argument;
	// arg = malloc(sizeof(char) * (size_of_argument + 1));
	arg = malloc(sizeof(char) * SIZE);
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
