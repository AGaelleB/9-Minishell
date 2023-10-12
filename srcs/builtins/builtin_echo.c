/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:18:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/12 11:49:14 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_echo_fd(char **tab)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (tab[i] && ft_strncmp(tab[i], "-n", 2) == 0)
	{
		j = 2;
		while (tab[i][j] == 'n')
			j++;
		if (tab[i][j] != '\0')
			break ;
		i++;
	}
	while (tab[i])
	{
		ft_putstr_fd(tab[i], STDOUT_FILENO);
		if (tab[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (j == 0)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
