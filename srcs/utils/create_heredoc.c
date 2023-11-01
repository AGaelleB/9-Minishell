/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 10:39:39 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/01 16:20:15 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	aleatori_char(void)
{
	char	buff[4];
	int		nbr;
	int		fd;

	fd = open("/dev/random", O_RDONLY);
	if (fd < -1)
		return (-1);
	read(fd, buff, 4);
	nbr = *(int *)buff;
	if (nbr < 0)
		nbr++;
	if (nbr < 0)
		nbr = nbr * (-1);
	return ('a' + nbr % 26);
}

char	*create_heredoc(void)
{
	char	*heredoc;
	int		i;

	i = 0;
	heredoc = malloc(sizeof(char) * 11);
	if (!heredoc)
		return (NULL);
	heredoc[10] = '\0';
	while (i < 10)
	{
		heredoc[i] = (char)aleatori_char();
		i++;
	}
	return (heredoc);
}
