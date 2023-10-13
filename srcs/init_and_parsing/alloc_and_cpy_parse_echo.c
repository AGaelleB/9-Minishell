/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/10/13 11:05:54 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_skip_redirection_and_file(char *input, int *i)
{
	if (input[*i] == '>' || input[*i] == '<')
	{
		(*i)++;
		skip_spaces_echo(input, i);
		while (input[*i] && input[*i] != ' ' && input[*i] != '>'
			&& input[*i] != '<')
		{
			if (input[*i] == '\'' || input[*i] == '\"')
				break ;
			(*i)++;
		}
		skip_spaces_echo(input, i);
	}
}

/*Fonction 2: Allocation de l'argument et copie des caractères*/
char	*ft_allocate_and_copy(char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(ft_strlen(input) + 1);
	if (!(arg))
		exit(EXIT_FAILURE); // Handle memory allocation failure
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if ((input[*i] == '>' || input[*i] == '<') && !double_quote
			&& !single_quote)
			break ;
		else if (input[*i] == ' ' && !double_quote && !single_quote)
			break ;
		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	ft_skip_redirection_and_file(input, i);
	return (arg);
}
