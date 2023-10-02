/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:18:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/02 10:21:06 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_echo(char **tab)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (tab[i])
	{
		while (tab[i][j])
		{
			printf("%c", tab[i][j]);
			j++;
		}
		printf(" ");
		j = 0;
		i++;
	}
	printf("\n");
	return (0);
}

