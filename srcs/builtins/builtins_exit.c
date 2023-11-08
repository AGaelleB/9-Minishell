/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 15:41:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 17:31:06 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	validate_exit_status(char *exit_status_str)
{
	int	i;

	i = 0;
	while (exit_status_str[i])
	{
		if (!(ft_isdigit(exit_status_str[i])))
		{
			write(2, "exit\n", 5);
			ft_putstr_fd("minishell: exit: ", 2);
			write(2, exit_status_str, ft_strlen(exit_status_str));
			ft_putstr_fd(": numeric argument required\n", 2);
			g_exit_status = 2;
			exit(g_exit_status);
		}
		i++;
	}
	return (ft_atoi(exit_status_str));
}

static void	handle_exit_with_status(char *input)
{
	char	*exit_status_str;
	int		exit_status;

	exit_status_str = input + 4;
	if (*exit_status_str == ' ')
	{
		exit_status_str = ft_strtrim(exit_status_str, " ");
		exit_status = validate_exit_status(exit_status_str);
		free(exit_status_str);
		write(2, "exit\n", 5);
		g_exit_status = exit_status;
		exit(g_exit_status);
	}
}

int	ft_builtin_write_exit(char *input)
{
	char	*str;

	str = ft_strtrim(input, " ");
	if (ft_strcmp_minishell(str, "exit") == 0)
	{
		write_exit_simple();
		free(str);
		return (0);
	}
	if (ft_strncmp(str, "exit", 4) == 0)
	{
		handle_exit_with_status(str);
		free(str);
		return (0);
	}
	free(str);
	return (0);
}
