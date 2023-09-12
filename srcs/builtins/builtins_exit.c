/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:39:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/11 17:39:30 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_exit(char *input)
{
	if (ft_strcmp_minishell(input, "exit") == 0)
	{
		printf("exit\n");
		return (-1);
	}
	return (0);
}

int	ft_builtin_(char *input)
{
	if (ft_strcmp_minishell(input, "exit") == 0)
	{
		printf("exit\n");
		return (-1);
	}
	return (0);
}
