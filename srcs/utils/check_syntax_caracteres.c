/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_caracteres.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:08:08 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/25 17:40:30 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_valid_caractere_filename(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '#'
		|| c == '(' || c == ')' || c == '!' || c == ';'
		|| c == '?' || c == '&' || c == '*' || c == '\\')
	{
		return (1);
	}
	else
		return (0);
}
