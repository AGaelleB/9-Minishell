/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:32:38 by bfresque          #+#    #+#             */
/*   Updated: 2023/09/15 11:24:53 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
}

void close_fd()
{
	int			fd;
	
	fd = 3;
	while (fd < 100)
	{
		close(fd);
		fd++;
	}
}
