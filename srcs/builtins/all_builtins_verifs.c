/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_builtins_verifs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:02:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/02 12:29:56 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_input(char *input)
{
	if (ft_builtin_enter(input) != 0)
		return (2);
	if (ft_builtin_write_exit(input) != 0)
	{
		free(input);
		exit(0);
	}
	if (ft_is_all_space(input) != 0)
		return (2);
	if (ft_strcmp_minishell(input, "\"\"") == 0
		|| ft_strcmp_minishell(input, "\'\'") == 0)
	{
		printf("minishell: : command not found\n");
		free(input);
		return (2);
	}
	return (0);
}

int	builtins_verif(t_command *current, t_env *env)
{
	while (current)
	{
		if (check_pwd(current))
			return (1);
		if (check_echo(current, env))
			return (1);
		if (check_env(current, env))
			return (1);
		if (check_cd(current))
			return (1);
		if (check_unset(current))
			return (1);
		if (check_export(current, env))
			return (1);
		current = current->next;
	}
	return (0);
}
