/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:20:47 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/07 14:23:57 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_status;

static char	*get_exit_status_str(void)
{
	char	*str;

	str = ft_itoa(g_exit_status);
	if (!str)
		exit(EXIT_FAILURE);
	return (str);
}

static void	extract_variable_name(char *input, int *i, int *start, char **str)
{
	int	end;

	end = *start;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
	{
		(*i)++;
	}
	end = *i - *start;
	*str = ft_substr(input, *start, end);
	if (!(*str))
		exit(EXIT_FAILURE);
}

static void	append_env_value(char *env_value, char *arg, int *arg_idx)
{
	int	j;

	j = 0;
	while (env_value[j])
	{
		arg[(*arg_idx)++] = env_value[j];
		j++;
	}
}

static void	handle_dollar(t_env *env, char *input, int *i, char *arg, int *arg_idx)
{
	char	*str;
	char	*env_value;
	int		start;

	start = *i + 1;
	if (input[start] == '?')
	{
		str = get_exit_status_str();
		append_env_value(str, arg, arg_idx);
		free(str);
		*i += 2;
	}
	else
	{
		(*i)++;
		extract_variable_name(input, i, &start, &str);
		env_value = get_env_value(env, str);
		if (env_value)
			append_env_value(env_value, arg, arg_idx);
		// free(str); //double free
	}
}

void	handle_arg_value(t_env *env, char *input, int *i, char *arg, int *arg_idx)
{
	(void)env;
	if (input[*i] == '$')
		handle_dollar(env, input, i, arg, arg_idx);
	else
		arg[(*arg_idx)++] = input[(*i)++];
}
