/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/25 16:34:19 by abonnefo         ###   ########.fr       */
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

int	write_in_fd(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		// signal(SIGINT, ft_builtin_ctrl_c);  // Réinitialisez le gestionnaire après readline
		if (global_ctrl_c_pressed) // NEW
		{
			free(line);
			return (-1); // Indique que Ctrl+C a été pressé
		}
		if (!line)
		{
			global_ctrl_c_pressed = 1;
			return (45);
		}
		if (ft_strcmp_minishell(line, delimiter) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	return (0);
}

void	add_to_heredocs_list(t_command *current, char *heredoc_name)
{
	int	i;
	
	i = 0;
	if (!current->heredocs)
	{
		current->heredocs = malloc(sizeof(char*) * 2);
		if (!current->heredocs)
			return ;
		current->heredocs[0] = ft_strdup(heredoc_name);
		current->heredocs[1] = NULL;
		return ;
	}
	while (current->heredocs[i])
		i++;
	current->heredocs = ft_realloc(current->heredocs, sizeof(char*) * (i + 1), sizeof(char*) * (i + 2));
	current->heredocs[i] = ft_strdup(heredoc_name);
	current->heredocs[i + 1] = NULL;
}

t_token	*handle_multiple_heredocs(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	while (token && ft_strcmp_minishell(token->split_value, "<<") == 0)
	{
		delimiter = token->next->split_value;
		if (current->heredoc)
			free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
		add_to_heredocs_list(current, current->heredoc);
		write_in_fd(fd, delimiter);
		fd = open(current->heredoc, O_RDONLY);
		current->fd_in = fd;
		if (current->fd_in == -1)
		{
			perror("minishell: EOF");
			exit(-1);
		}
		token = token->next->next;
	}
	return(token);
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
