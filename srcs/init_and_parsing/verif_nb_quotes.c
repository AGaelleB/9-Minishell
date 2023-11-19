/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_nb_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 14:27:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/19 14:36:38 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_quotes(char *input)
{
	bool	double_quote;
	bool	single_quote;
	int		i;

	double_quote = false;
	single_quote = false;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' && !single_quote)
			double_quote = !double_quote;
		else if (input[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		i++;
	}
	return (single_quote || double_quote);
}

int	verif_nb_quote(char *input)
{
	if (check_quotes(input))
	{
		printf("minishell: missing quote\n");
		return (g_exit_status = 2);
	}
	return (0);
}
