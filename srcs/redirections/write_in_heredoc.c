/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_in_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 11:46:59 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 10:54:00 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_status;

// static void	handle_heredoc_signals(t_command *current, char *line)
// {
// 	if (g_exit_status == 130)
// 	{
// 		clean_heredoc_files(current);
// 		free(line);
// 		exit(130);
// 	}
// }

static int	check_line_with_delimiter(char *line, char *delimiter, int fd)
{
	if (ft_strcmp_minishell(line, delimiter) == 0)
		return (1);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	return (0);
}

int	write_in_fd(int fd, char *delimiter, t_command *current)
{
	char	*line;
	int		i;

	i = 0;
	handle_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		(void)current;
		// handle_heredoc_signals(current, line);
		if (ctrl_d_heredoc(line, i, delimiter) == 45)
		{
			free(line);
			return (45);
		}
		if (check_line_with_delimiter(line, delimiter, fd))
			break ;
		i++;
		free(line);
	}
	if (line)
		free(line);
	return (0);
}
