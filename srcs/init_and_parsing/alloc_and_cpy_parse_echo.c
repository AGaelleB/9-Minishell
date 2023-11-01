/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/01 15:28:32 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes_echo(char *str, int *i,
	bool *double_quote, bool *in_quote)
{
	if (!*in_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		(*i) += 2;
	else if (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
		(*i) += 2;
	if (!*double_quote && str[*i] == '\'')
		*in_quote = !*in_quote;
	else if (!*in_quote && str[*i] == '\"')
		*double_quote = !*double_quote;
	if (!*in_quote && str[*i] == '\"')
		(*i)++;
	else if (!*double_quote && str[*i] == '\'')
		(*i)++;
}

int	is_redirection(char c)
{
	return (c == '>' || c == '<');
}

void	ft_skip_redirection_and_file(char *input, int *i)
{
	bool single_quote = false;
	bool double_quote = false;
	if (is_redirection(input[*i]))
	{
		(*i)++;
		skip_spaces_echo(input, i);
		while (input[*i] && !is_redirection(input[*i]))
		{
			if (input[*i] == '\'')
				single_quote = !single_quote;
			else if (input[*i] == '\"')
				double_quote = !double_quote;
			if (input[*i] == ' ' && !single_quote && !double_quote)
				break;
			if ((input[*i] == '\'' && !single_quote) || (input[*i] == '\"' && !double_quote))
			{
				(*i)++;
				continue;
			}
			(*i)++;
		}
		skip_spaces_echo(input, i);
	}
}

void	handle_arg_value(t_env *env, char *input, int *i, char *arg,
	int *arg_idx)
{
	char	*str;
	int		j;
	int		start;

	j = 0;
	start = *i + 1;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\''
		&& input[*i] != '\"')
	{
		(*i)++;
		if (input[*i] != '\"' && input[*i] != '\'')
			j++;
	}
	str = ft_substr(input, start, j);
	append_env_value_to_arg(get_env_value(env, str), arg, arg_idx);
}

char	*ft_allocate_and_copy(t_env *env, char *input, int *i,
	int *arg_idx)
{
	char	*arg;
	int		len;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;


	len = count_arg_length(input, *i); 
	arg = malloc(len + 1);
	if (!arg)
		exit(EXIT_FAILURE);
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if (is_redirection(input[*i]) && !double_quote && !single_quote)
		{
			ft_skip_redirection_and_file(input, i);
			continue;
		}
		else if (input[*i] == ' ' && !double_quote && !single_quote)
			break ;
		else if (input[*i] == '$' && !single_quote)
			handle_arg_value(env, input, i, arg, arg_idx);
		else
		{
			arg[(*arg_idx)++] = input[*i];
			(*i)++;
		}
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	return (arg);
}
