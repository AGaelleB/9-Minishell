/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_valid_identifier_export.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:48:37 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 16:49:05 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_first_char_valid(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static int	is_char_valid(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '='
		|| c == '\'' || c == '\"' || c == ' ');
}

static void	print_invalid_identifier(char *str, int index)
{
	ft_putstr_fd("minishell: export: \'", 2);
	write(2, &str[index], 1);
	ft_putstr_fd("\': not a valid identifier\n", 2);
}

int	check_valid_identifier_export(char *str)
{
	int	i;

	i = 0;
	if (!is_first_char_valid(str[i]))
	{
		print_invalid_identifier(str, i);
		g_exit_status = 1;
		return (g_exit_status);
	}
	i++;
	while (str[i])
	{
		if (!is_char_valid(str[i]))
		{
			print_invalid_identifier(str, i);
			g_exit_status = 1;
			return (g_exit_status);
		}
		i++;
	}
	return (0);
}
