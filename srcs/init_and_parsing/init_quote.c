/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 10:44:53 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 16:42:47 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	verif_nb_single_quote(char *str)
{
	int	i;
	int	count_quote;

	i = 0;
	count_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			count_quote++;
		i++;
	}
	if (count_quote % 2 == 0)
		return (0);
	else
		return (-1);
}

int	verif_nb_double_quote(char *str)
{
	int	i;
	int	count_quote;

	i = 0;
	count_quote = 0;
	while (str[i])
	{
		if (str[i] == '"')
			count_quote++;
		i++;
	}
	if (count_quote % 2 == 0)
		return (0);
	else
		return (-1);
}

int	verif_nb_quote(char *input)
{
	if ((verif_nb_single_quote(input) != 0)
		|| (verif_nb_double_quote(input) != 0))
	{
		printf("minishell: miss quote\n");
		free(input);
		return (-1);
	}
	return (0);
}

bool	contains_single_quote(char *str)
{
	while (*str)
	{
		if (*str == '\'')
			return (true);
		str++;
	}
	return (false);
}

bool	contains_double_quote(char *str)
{
	while (*str)
	{
		if (*str == '"')
			return (true);
		str++;
	}
	return (false);
}
