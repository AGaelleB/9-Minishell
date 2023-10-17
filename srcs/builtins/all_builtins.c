/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:02:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/17 16:01:19 by abonnefo         ###   ########.fr       */
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

static int	check_pwd(t_command *current)
{
	if (ft_strcmp_minishell(current->command, "pwd") == 0)
	{
		ft_builtin_pwd(STDOUT_FILENO);
		return (1);
	}
	return (0);
}

static int	check_echo(t_command *current, t_env *env)
{
	char	**tab;

	if (ft_strncmp(current->command, "echo ", 5) == 0)
	{
		tab = parse_input_quote_echo(env, current->command);
		ft_builtin_echo(tab);
		return (1);
	}
	return (0);
}

static int	check_env(t_command *current, t_env *env)
{
	if (ft_strcmp_minishell(current->command, "env") == 0)
	{
		ft_builtin_env(env);
		return (1);
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
		current = current->next;
	}
	return (0);
}
