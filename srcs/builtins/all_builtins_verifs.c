/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_builtins_verifs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:02:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/04 15:10:43 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_input(t_env *env, char *input)
{
	if (ft_builtin_enter(input) != 0)
		return (2);
	if (ft_builtin_write_exit(env, input) != 0)
	{
		free(input);
		return (2);
	}
	if (ft_is_all_space(input) != 0)
		return (2);
	if (ft_strcmp_minishell(input, "\"\"") == 0
		|| ft_strcmp_minishell(input, "\'\'") == 0)
	{
		printf("minishell: : command not found\n");
		free(input);
		g_exit_status = 127;
		return (2);
	}
	return (0);
}

int	check_g_expander(t_command *current)
{
	if (current->command[1] == '?')
	{
		write(1, "minishell: ", 11);
		ft_putnbr_fd(g_exit_status, 1);
		write(1, ": command not found\n", 21);
		g_exit_status = 127;
		return (1);
	}
	return (0);
}

int	ret_place_var(t_command *current, t_env *env, char *str)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (current->command[i]
		&& ((ft_isalnum(current->command[i]))
			|| (current->command[i] == '_')))
	{
		str[j++] = current->command[i++];
	}
	str[j] = '\0';
	i = find_env_var(env, str);
	return (i);
}

int	check_expand(t_command *current, t_env *env)
{
	int		i;
	char	*str;

	if (current->command[0] == '$')
	{
		if (check_g_expander(current) == 1)
			return (1);
		str = malloc(sizeof(char) * SIZE);
		if (!str)
			return (-1);
		i = ret_place_var(current, env, str);
		if (i != -1)
		{
			print_expander_cmd(env, str, i);
			free(str);
			g_exit_status = 127;
			return (1);
		}
		else
		{
			free(str);
			return (1);
		}
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
		if (check_expand(current, env))
			return (1);
		current = current->next;
	}
	return (0);
}
