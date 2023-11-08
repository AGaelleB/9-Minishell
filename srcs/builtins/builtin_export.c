/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 17:22:00 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export(char *str, int fd)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (str[i])
	{
		ft_putchar_fd(str[i], fd);
		if (str[i] == '=' && flag == 0)
		{
			flag = 1;
			ft_putchar_fd('\"', fd);
		}
		i++;
	}
	if (flag == 1)
		ft_putchar_fd('\"', fd);
}

static int	print_env_vars(t_env *env)
{
	int	i;

	i = 0;
	while (env->cpy_env[i])
	{
		ft_putstr_fd("export ", STDOUT_FILENO);
		print_export(env->cpy_env[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (1);
}

int	ft_builtin_export(char **args, t_env *env)
{
	int		arg_idx;
	int		i;
	char	*str;

	i = 0;
	if (!args[1])
		return (print_env_vars(env));
	arg_idx = 1;
	while (args[arg_idx])
	{
		str = handle_quotes_export(args[arg_idx]);
		if (check_valid_identifier_export(str))
		{
			if (update_var_env(env, str) == 1)
				return (1);
			arg_idx++;
		}
		else
			break ;
	}
	add_var_env(env, i, str);
	return (g_exit_status);
}
