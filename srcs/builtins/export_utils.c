/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 10:16:16 by bfresque          #+#    #+#             */
/*   Updated: 2023/12/08 16:02:46 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_exit_status(char **str_cpy, int *y, int *i)
{
	char	*exit_stat;
	int		j;

	j = 0;
	exit_stat = ft_itoa(g_exit_status);
	while (exit_stat[j])
		(*str_cpy)[(*y)++] = exit_stat[j++];
	free(exit_stat);
	(*i)++;
}

char	*check_none_var(char *str)
{
	char	*str_cpy;
	int		i;
	int		y;

	y = 0;
	i = 0;
	str_cpy = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!str_cpy)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				handle_exit_status(&str_cpy, &y, &i);
			else
				skip_var_name(str, &i);
		}
		else
			str_cpy[y++] = str[i];
		i++;
	}
	str_cpy[y] = '\0';
	return (str_cpy);
}

int	apply_var_name(t_export *export, char **str, t_env *env, char *var_name)
{
	if (!(export->ret))
	{
		export->ret = malloc(sizeof(*str)
				* (ft_strlen(env->cpy_env[export->j]) + ft_strlen(*str) + 2));
		if (!export->ret)
			return (-1);
	}
	begin_var(export, *str);
	copy_env_in_return(export, env);
	*str = re_init_var_str(export, *str);
	if (!(*str))
		return (free(var_name), 0);
	return (1);
}

int	expand_variable(t_export *export, char **str, t_env *env)
{
	char	*var_name;

	while (1)
	{
		export->k = 0;
		var_name = extract_var_name(*str);
		export->j = find_env_var(env, var_name);
		if (export->j != -1)
		{
			if (apply_var_name(export, str, env, var_name) == 0)
				return (0);
		}
		else
		{
			*str = check_none_var(*str);
			update_var_env(env, *str);
			add_var_env(env, export->i, *str);
			free(var_name);
			return (1);
		}
		free(var_name);
	}
}

int	export_expander(t_export *export, char *str, t_env *env)
{
	char	*str_cpy;

	str_cpy = ft_strdup(str);
	if (expand_variable(export, &str, env) == 0)
	{
		update_var_env(env, str_cpy);
		export->ret[export->l] = '\0';
		add_var_env(env, export->i, export->ret);
		free(str_cpy);
		free(str);
		return (g_exit_status);
	}
	free(str_cpy);
	return (g_exit_status);
}
