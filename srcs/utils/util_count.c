/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:05:15 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 12:06:03 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipe(char *input)
{
	int	count_pipe;
	int	i;

	count_pipe = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			count_pipe++;
		i++;
	}
	return (count_pipe);
}
