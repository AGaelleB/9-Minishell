/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/24 15:15:40 by abonnefo         ###   ########.fr       */
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
	heredoc[10] = '\0';
	while (i < 10)
	{
		heredoc[i] = (char)aleatori_char();
		i++;
	}
	return (heredoc);
}

int	write_in_fd(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (45);
		if (ft_strcmp_minishell(line, delimiter) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	return (0);
}

int	redirect_heredoc(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	fd = -1;
	delimiter = token->next->split_value;
	if (fd == -1)
	{
		if (current->heredoc)
			free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
	}
	write_in_fd(fd, delimiter);
	fd = open(current->heredoc, O_RDONLY);
	current->fd_in = fd;
	if (current->fd_in == -1)
	{
		perror("minishell: EOF");
		exit(-1);
	}
	return (0);
}


/* 
bash-5.1$ cat << UN << DEUX << TROIS
> un
> UN
> deux
> DEUX
> trois
> TROIS
trois

minishell$> cat << UN << DEUX << TROIS
> un
> UN
> un
> > un
> un
 */