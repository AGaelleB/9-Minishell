/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/02 16:52:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_status;

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

static char	*get_exit_status_str(void)
{
	char	*str;

	str = ft_itoa(g_exit_status);
	if (!str)
		exit(EXIT_FAILURE);
	return (str);
}

static void	handle_dollar(char *input, int *i, char *arg, int *arg_idx)
{
	char	*str;
	int		j;
	// int		start;

	if (input[*i + 1] == '?')
	{
		str = get_exit_status_str();
		j = -1;
		while (str[++j])
			arg[(*arg_idx)++] = str[j];
		free(str);
		*i += 2;
	}
	else
	{
		*i += 1;
		j = *i;
		while (input[*i] && input[*i] != ' ' && input[*i] != '\'' && input[*i] != '\"')
			(*i)++;
		str = ft_substr(input, j, *i - j);
		if (!str)
			exit(EXIT_FAILURE);
		append_env_value_to_arg(str, arg, arg_idx);
		free(str);
	}
}

void	handle_arg_value(t_env *env, char *input, int *i, char *arg, int *arg_idx)
{
	(void)env;
	if (input[*i] == '$')
		handle_dollar(input, i, arg, arg_idx);
	else
		arg[(*arg_idx)++] = input[(*i)++];
}


// void	handle_arg_value(t_env *env, char *input, int *i, char *arg, int *arg_idx)
// {
// 	char	*str;
// 	int		j;
// 	int		start;

// 	j = 0;
// 	start = *i + 1;
// 	while (input[*i] && input[*i] != ' ' && input[*i] != '\''
// 		&& input[*i] != '\"')
// 	{
// 		(*i)++;
// 		if (input[*i] != '\"' && input[*i] != '\'')
// 			j++;
// 	}
// 	str = ft_substr(input, start, j);
// 	append_env_value_to_arg(get_env_value(env, str), arg, arg_idx);
// }

char	*ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(sizeof(char *) * SIZE);
	if (!arg)
		exit(EXIT_FAILURE);
	*arg_idx = 0;
	while (input[*i] && (double_quote || single_quote || input[*i] != ' '))
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if (is_redirection(input[*i]) && !double_quote && !single_quote)
			ft_skip_redirection_and_file(input, i);
		else if (input[*i] == '$' && !single_quote)
			handle_arg_value(env, input, i, arg, arg_idx);
		else
			arg[(*arg_idx)++] = input[(*i)++];
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	return (arg);
}
