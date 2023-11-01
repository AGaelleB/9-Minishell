/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 11:18:16 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/01 16:11:54 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_valid_n_option(char *str)
{
	int	j;

	if (ft_strncmp(str, "-n", 2) != 0)
		return (0);
	j = 2;
	while (str[j] == 'n')
		j++;
	if (str[j] != '\0')
		return (0);
	return (1);
}

static int	handle_newline(char **tab, int *i)
{
	int	newline;

	newline = 1;
	while (tab[*i] && tab[*i][0] == '-')
	{
		if (!is_valid_n_option(tab[*i]))
			break ;
		newline = 0;
		(*i)++;
	}
	return (newline);
}

static void	print_tab_content(char **tab, int i)
{
	while (tab[i])
	{
		ft_putstr_fd(tab[i], STDOUT_FILENO);
		if (tab[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
}

int	ft_builtin_echo(char **tab)
{
	int	i;
	int	newline;

	i = 1;
	newline = handle_newline(tab, &i);
	print_tab_content(tab, i);
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
