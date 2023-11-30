/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:00:41 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/30 16:46:09 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*create_str_from_arg(char *arg)
{
	int		i;
	char	*str;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, arg, i + 1);
	return (str);
}

int	update_var_env(t_env *env, char *arg)
{
	int		i;
	char	*str;

	str = create_str_from_arg(arg);
	if (!str)
		return (-1);
	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], str, ft_strlen(str)) == 0
			&& ((env->cpy_env[i][ft_strlen(str)] == '=')
			|| !(env->cpy_env[i][ft_strlen(str)])))
		{
			remove_env_var(env, i);
			free(str);
			return (1);
		}
		i++;
	}
	free(str);
	return (0);
}

int	add_var_env(t_env *env, int i, char *str)
{
	i = 0;
	while (env->cpy_env[i])
	{
		if (env->cpy_env[i + 1] == NULL)
		{
			env->cpy_env[i + 1] = str;
			env->cpy_env[i + 2] = NULL;
			// free(str);
			return (1);
		}
		i++;
	}
	return (0);
}
