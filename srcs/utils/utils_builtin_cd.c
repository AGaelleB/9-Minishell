/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtin_cd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:43:09 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/01 15:44:19 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_get_env(const char *name, t_env *env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env->cpy_env[i])
	{
		if ((ft_strncmp(env->cpy_env[i], name, len) == 0)
			&& (env->cpy_env[i][len] == '='))
			return (&env->cpy_env[i][len + 1]);
		i++;
	}
	return (NULL);
}

int	pass_find_var_name(t_env *env, char *var_name, int *i, int j)
{
	while (env->path_to_change
		&& (ft_isalnum(env->path_to_change[*i])
			&& env->path_to_change[*i] != '_'
			&& env->path_to_change[*i] != ':'
			&& env->path_to_change[*i] != '$'))
	{
		var_name[j++] = env->path_to_change[(*i)++];
	}
	return (j = find_env_var(env, var_name));
}
