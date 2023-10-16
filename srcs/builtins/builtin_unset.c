/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:58:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/16 16:53:35 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_builtin_unset(char **args, t_env *env)
{
	int	i;
	int	arg_idx;

	arg_idx = 1;
	env->flag_builtin = true;
	if (!args[1])
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return ;
	}
	// if(ft_strcmp_minishell(args[1], "PATH") == 0)
	while (args[arg_idx])
	{
		i = 0;
		while (env->cpy_env[i])
		{
			if (ft_strncmp(env->cpy_env[i], args[arg_idx],
				ft_strlen(args[arg_idx])) == 0
				&& env->cpy_env[i][ft_strlen(args[arg_idx])] == '=')
			{
				free(env->cpy_env[i]);
				// Shift all subsequent items up
				while (env->cpy_env[i + 1])
				{
					env->cpy_env[i] = env->cpy_env[i + 1];
					i++;
				}
				env->cpy_env[i] = NULL;
				break;
			}
			i++;
		}
		arg_idx++;
	}
}
