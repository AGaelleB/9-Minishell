/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/10/25 17:25:32 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	*env_var_exists(t_env *env, char *var)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], var, ft_strlen(var)) == 0
			&& env->cpy_env[i][ft_strlen(var)] == '=')
		{
			j = ft_strlen(var) + 1; // +1 pour sauter le '='
			str = ft_strdup(&env->cpy_env[i][j]);
			if (!str)
				return (NULL);
			return (str);
		}
		i++;
	}
	return (NULL);
}

char	*get_env_value(t_env *env, char *str)
{
	char	*value;

	value = env_var_exists(env, str);
	if (value)
	{
		free(str);
		return (value);
	}
	free(str);
	return (NULL);
}

void	append_env_value_to_arg(char *value, char *arg, int *arg_idx)
{
	int	j;

	j = 0;
	while (value && value[j])
	{
		arg[(*arg_idx)++] = value[j];
		j++;
	}
	free(value);
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
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(sizeof(input) * 1000);
	if (!arg)
		exit(EXIT_FAILURE);
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if ((input[*i] == '>' || input[*i] == '<') && !double_quote
			&& !single_quote)
			break ;
		else if (input[*i] == ' ' && !double_quote && !single_quote)
			break ;
		else if (input[*i] == '$' && !single_quote)
			handle_arg_value(env, input, i, arg, arg_idx);
		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	ft_skip_redirection_and_file(input, i);
	return (arg);
}
