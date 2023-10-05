/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 10:44:53 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 16:29:44 by abonnefo         ###   ########.fr       */
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
	// printf("count_quote = %d\n", count_quote); //////
	if (count_quote % 2 == 0)
		return (0);
	else
		return (-1);
}



// t_e_quote	update_quote_status(char c, t_e_quote quote)
// {
// 	if (c == '"')
// 	{
// 		if (quote == NONE)
// 			return (DOUBLE);
// 		else if (quote == DOUBLE)
// 			return (NONE);
// 	}
// 	else if (c == '\'')
// 	{
// 		if (quote == NONE)
// 			return (SINGLE);
// 		else if (quote == SINGLE)
// 			return (NONE);
// 	}
// 	return (quote);
// }
