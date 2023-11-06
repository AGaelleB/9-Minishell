/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:24 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 09:49:10 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_arg_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
		i++;
	return (i);
}

void	copy_env(t_env *env, char **envp)
{
	int		i;

	i = count_arg_tab(envp);
	env->cpy_env = malloc(sizeof(char *) * (i + 1));
	if (!env->cpy_env)
	{
		free(env);
		return ;
	}
	i = 0;
	while (envp[i])
	{
		env->cpy_env[i] = ft_strdup(envp[i]); // don't forget to free this later
		if (!env->cpy_env[i])
		{
			ft_free_tab(env->cpy_env);
			return ;
		}
		i++;
	}
	env->cpy_env[i] = NULL;
	env->flag_error = false;
}

void	ft_builtin_env(t_env *env)
{
	int		i;

	i = 0;
	while (env->cpy_env[i])
	{
		ft_putstr_fd(env->cpy_env[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}
