/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollars.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 14:20:47 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/30 14:33:24 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_exit_status_str(void)
{
	char	*str;

	str = ft_itoa(g_exit_status);
	if (!str)
		return (NULL);
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
		return ;
}

static void	append_env_value(t_arg_handler *arg_handler, char *env_value)
{
	while (*env_value)
		arg_handler->arg[(*arg_handler->arg_idx)++] = *env_value++;
}

static void	handle_dollar(t_arg_handler *arg_handler)
{
	char	*str;
	char	*env_value;
	int		start;

	start = *arg_handler->i + 1;
	if (arg_handler->input[start] == '?')
	{
		str = get_exit_status_str();
		append_env_value(arg_handler, str);
		free(str);
		*arg_handler->i += 2;
	}
	else
	{
		(*arg_handler->i)++;
		extract_variable_name(arg_handler->input, arg_handler->i, &start, &str);
		env_value = get_env_value(arg_handler->env, str);
		if (env_value)
			append_env_value(arg_handler, env_value);
		free(env_value);
	}
}

void	handle_arg_value(t_arg_handler *arg_handler)
{
	if (arg_handler->input[*arg_handler->i] == '$'
		&& arg_handler->input[*arg_handler->i + 1] != '$'
		&& (ft_isalpha(arg_handler->input[*arg_handler->i + 1]) == 1
			|| arg_handler->input[*arg_handler->i + 1] == '?'))
		handle_dollar(arg_handler);
	else
		arg_handler->arg[(*arg_handler->arg_idx)++]
			= arg_handler->input[(*arg_handler->i)++];
}
