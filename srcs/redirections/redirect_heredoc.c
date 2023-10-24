/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/24 18:18:55 by abonnefo         ###   ########.fr       */
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
	// printf("Je créé le heredoc : %s\n", heredoc); // PRINNNNT
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

void add_to_heredocs_list(t_command *current, char *heredoc_name) // NEW
{
	int	i;
	
	i = 0;
	// Si c'est la 1er fois que l'on ajoute un heredoc, j'init le tableau
	if (!current->heredocs)
	{
		current->heredocs = malloc(sizeof(char*) * 2);  // Espace pour un nom de fichier + NULL à la fin
		if (!current->heredocs)
			return ;
		current->heredocs[0] = ft_strdup(heredoc_name);
		current->heredocs[1] = NULL;
		return ;
	}
	// Sinon, je trouve la taille actuelle du tableau
	while (current->heredocs[i])
		i++;
	// Redimensionnons le tableau pour ajouter un nouvel élément
	current->heredocs = realloc(current->heredocs, sizeof(char*) * (i + 2)); // Espace pour le nouvel élément + NULL à la fin
	// REALLOC EST FORBIDEN

	// Ajoutons le nouveau heredoc à la fin
	current->heredocs[i] = ft_strdup(heredoc_name);
	current->heredocs[i + 1] = NULL;
}

t_token	*handle_multiple_heredocs(t_command *current, t_token *token) // NEW
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
		add_to_heredocs_list(current, current->heredoc);  // fonction pour ajouter le nom du fichier à la liste
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


/* 
faire des tests qd on retire le PATH

chez nous : 

minishell$> cat << a << b << c
> coucou a
> a
> coucou b
> b
> coucou c
> c
coucou c
minishell$> unset PATH
minishell$> cat << a << b << c
> coucou a
> a
> coucou b
> b
> coucou c
> c
minishell: cat: No such file or directory



chez Rayan 

[42] $> cat << a << b << c
>coucou a
>a
>coucou b
>b
>coucou c
>c
coucou c
[42] $> unset PATH
[42] $> cat << a << b << c
>coucou a
>a
>coucou b
>b
>coucou c
>c
[42] $> 
 */