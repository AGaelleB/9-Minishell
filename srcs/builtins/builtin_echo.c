/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:18:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/02 14:48:11 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_echo(char **tab)
{
	int	i;
	int	j;
	int	flag_option;

	i = 1;
	flag_option = 0;
	while (tab[i] && ft_strncmp(tab[i], "-n", 2) == 0)
	{
		j = 2;
		while (tab[i][j] == 'n')
			j++;
		if (tab[i][j] != '\0')
			break ;
		flag_option = 1;
		i++;
	}
	while (tab[i])
	{
		printf("%s", tab[i]);
		if (tab[i + 1])
			printf(" ");
		i++;
	}
	if (!flag_option)
		printf("\n");
	return (0);
}

int	ft_builtin_echo_fd(char **tab)
{
	int	i;
	int	j;
	int	flag_option;

	i = 1;
	flag_option = 0;
	while (tab[i] && ft_strncmp(tab[i], "-n", 2) == 0)
	{
		j = 2;
		while (tab[i][j] == 'n')
			j++;
		if (tab[i][j] != '\0')
			break ;
		flag_option = 1;
		i++;
	}
	while (tab[i])
	{
		ft_putstr_fd(tab[i], STDOUT_FILENO);
		if (tab[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!flag_option)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
