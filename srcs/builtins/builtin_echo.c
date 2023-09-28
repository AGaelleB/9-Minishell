/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:18:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/28 16:26:52 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_echo(char *str)
{
	printf("*****notre echo*****\n");
	int	i;
	// int	y;
	// str++;
	// y = 0;
	i = 0;

	// if((str[i] == 'e') && (str[i + 1] == 'c') && (str[i + 2] == 'h') && (str[i + 3] == 'o'))
	// {
	// 	i = 5;
	// }

	while (str[i])
	{
		printf("%c", str[i]);
		// if (str[i + 1])
		// 	printf(" ");
		i++;
	}
	// if (y == 0)
		printf("\n");
	// exit(0);
	return (0);
}

