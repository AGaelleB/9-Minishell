/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:23:27 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 17:26:58 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_exit_simple(void)
{
	g_exit_status = 0;
	write(1, "exit\n", 5);
	exit(g_exit_status);
}

int	ft_builtin_enter(char *input)
{
	if (ft_strcmp_minishell(input, "") == 0)
	{
		free(input);
		return (1);
	}
	return (0);
}

int	ft_is_all_space(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] != ' ')
			return (0);
		i++;
	}
	free(input);
	return (1);
}

void	ft_builtin_ctrl_d(char *input)
{
	if (!input)
	{
		write(1, "exit", 5);
		write(1, "\n", 1);
		ft_close_all_fd();
		exit(0);
	}
}
