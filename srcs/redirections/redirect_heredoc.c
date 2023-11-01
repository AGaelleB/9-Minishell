/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/01 11:20:16 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	write_in_fd(int fd, char *delimiter, t_command *current)
{
	char	*line;
	int		i;

	i = 0;
	handle_signals_heredoc();
	while (1)
	{
		line = readline("> ");
		if (global_ctrl_c_pressed == 130)
		{
			clean_heredoc_files(current);
			free(line);
			exit(130);
		}
		if (ctrl_d_heredoc(line, i, delimiter) == 45)
			return (45);
		if (ft_strcmp_minishell(line, delimiter) == 0)
			break ;
		
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		i++;
		free(line);
	}
	if (line)
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
		delimiter = extract_filename_heredoc(current->command);
		// if (current->heredoc) // malloc(): unaligned fastbin chunk detected 3
		// 	free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
		add_to_heredocs_list(current, current->heredoc);
		write_in_fd(fd, delimiter, current);
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

