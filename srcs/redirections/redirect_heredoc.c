/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/17 15:33:57 by bfresque         ###   ########.fr       */
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

char	*create_file_name(void)
{
	char	*file_name;
	int		i;

	i = 0;
	file_name = malloc(sizeof(char) * 11);
	file_name[10] = '\0';
	while (i < 10)
	{
		file_name[i] = (char)aleatori_char();
		i++;
	}
	return (file_name);
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
		// if (str[0] != '\0') // A FAIRE !!!
		// {
			// permet de supprimer le file creer, avec l'environement
		// }
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
	char	*file_name;
	int		fd;

	file_name = NULL;
	fd = -1;
	delimiter = token->next->split_value;
	if (fd == -1)
	{
		if (file_name)
			free(file_name);
		file_name = create_file_name();
		fd = open(file_name, O_CREAT | O_EXCL | O_RDWR, 0644);
	}
	write_in_fd(fd, delimiter);
	fd = open(file_name, O_RDONLY);
	current->fd_in = fd;
	free_file_name(current->file_name);
	if (current->fd_in == -1)
	{
		perror("minishell: EOF");
		exit(-1);
	}
	current->file_name = file_name;
	return (0);
}
