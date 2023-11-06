/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 15:41:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 16:29:45 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_write_exit(char *input)
{
	char	*str;

	str = ft_strtrim(input, " ");
	if (ft_strcmp_minishell(str, "exit") == 0)
	{
		free(str);
		printf("exit\n");
		return (1);
	}
	free(str);
	return (0);
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
		ft_close_all_fd();
		write(1, "exit\n", 5);
		exit(0);
	}
}
