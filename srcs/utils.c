/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:32:38 by bfresque          #+#    #+#             */
/*   Updated: 2023/09/11 14:33:07 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void count_and_set_pipes(char *input, t_command *command)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			count++;
		i++;
	}
	command->nb_pipes = count;
	// printf("nombre de pipes commande : %d\n", command->nb_pipes);
}