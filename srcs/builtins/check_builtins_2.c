/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtins_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:44:17 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 10:49:14 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_env(t_command *current, t_env *env)
{
	if (ft_strcmp_minishell(current->command, "env") == 0)
	{
		ft_builtin_env(env);
		return (1);
	}
	return (0);
}

int	check_unset(t_command *current)
{
	if (ft_strncmp(current->command, "unset", 5) == 0)
	{
		if (current->nb_pipes != 0)
			return (1);
		return (1);
	}
	return (0);
}

int	check_export(t_command *current, t_env *env)
{
	(void)env;
	if (ft_strcmp_minishell(current->command, "export") == 0)
	{
		// ft_builtin_export(current->command_arg, env);
		return (1);
	}
	else if (ft_strncmp(current->command, "export ", 7) == 0)
		return (1);
	return (0);
}
