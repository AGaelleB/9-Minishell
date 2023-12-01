/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:15:24 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/01 16:02:00 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			j = ft_strlen(var) + 1;
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
}
