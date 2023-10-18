/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:58:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/18 16:54:31 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_env_var(t_env *env, char *arg)
{
	int	i;

	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], arg, ft_strlen(arg)) == 0
			&& env->cpy_env[i][ft_strlen(arg)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static void	remove_env_var(t_env *env, int idx)
{
	free(env->cpy_env[idx]);
	while (env->cpy_env[idx + 1])
	{
		env->cpy_env[idx] = env->cpy_env[idx + 1];
		idx++;
	}
	env->cpy_env[idx] = NULL;
}

void	ft_builtin_unset(char **args, t_env *env)
{
	int	idx;
	int	arg_idx;

	arg_idx = 1;
	env->flag_builtin = true;
	if (!args[1])
		return ;
	while (args[arg_idx])
	{
		idx = find_env_var(env, args[arg_idx]);
		if (idx != -1)
			remove_env_var(env, idx);
		arg_idx++;
	}
}
