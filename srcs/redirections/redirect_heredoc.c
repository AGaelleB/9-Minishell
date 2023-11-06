/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 12:36:39 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_to_heredocs_list(t_command *current, char *heredoc_name)
{
	int	i;

	i = 0;
	if (!current->heredocs)
	{
		current->heredocs = malloc(sizeof(char *) * 2);
		if (!current->heredocs)
			return ;
		current->heredocs[0] = ft_strdup(heredoc_name);
		current->heredocs[1] = NULL;
		return ;
	}
	while (current->heredocs[i])
		i++;
	current->heredocs = ft_realloc(current->heredocs,
			sizeof(char *) * (i + 1), sizeof(char *) * (i + 2));
	current->heredocs[i] = ft_strdup(heredoc_name);
	current->heredocs[i + 1] = NULL;
}

// char *cut_heredoc_name(char *cmd)
// {
// 	char **arg;
// 	arg = split(cmd, " ")
// }

t_token	*handle_multiple_heredocs(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	// int current_position = 0;
	while (token && ft_strcmp_minishell(token->split_value, "<<") == 0)
	{
		// printf("%stoken->split_value = %s %s\n", GREEN, token->next->split_value, RESET);

		printf("token->next->split_heredoc = %s\n", token->next->split_heredoc);
		delimiter = token->next->split_value;
		// delimiter = extract_filename_heredoc(current->command, &current_position);
		
		// delimiter = extract_filename_heredoc(token->next->split_value);

		// if (current->heredoc) // malloc(): unaligned fastbin chunk detected 3
		// 	free(current->heredoc);
		// printf("%sdelimiter = %s | %s", BLUE, delimiter, RESET);
		current->heredoc = create_heredoc();
		printf("%scurrent->heredoc = %s %s\n", MAGENTA, current->heredoc, RESET);
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
		add_to_heredocs_list(current, current->heredoc);
		write_in_fd(fd, delimiter, current);
		if (delimiter) //NEW
			free(delimiter); //NEW
		fd = open(current->heredoc, O_RDONLY);
		current->fd_in = fd;
		if (current->fd_in == -1)
		{
			perror("minishell: EOF");
			exit(-1);
		}
		// current_position += ft_strlen(delimiter) + 1;
		token = token->next->next;
	}
	return (token);
}
