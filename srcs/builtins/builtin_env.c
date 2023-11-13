/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:24 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/13 11:01:13 by bfresque         ###   ########.fr       */
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
	env->cpy_env = malloc(sizeof(char *) * (i + 11));
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
	g_exit_status = 0;
}
