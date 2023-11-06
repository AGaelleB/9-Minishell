/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 15:40:27 by abonnefo         ###   ########.fr       */
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


static void	skip_until_sequence(char *str, int *i, const char *seq)
{
	while (str[*i] && (str[*i] != seq[0] || strncmp(&str[*i], seq, strlen(seq)) != 0))
		(*i)++;
	// while(ft_strcmp_minishell(str, redir) == 0)
	// {
	// 	printf("str[%d] : %c\n", *i, str[*i]);
	// 	(*i)++;
	// }
}

static void	handle_quotes(char *str, int *i, bool *in_quote, bool *double_quote)
{
	if (!*in_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		(*i) += 2;
	else if (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
		(*i) += 2;
	if (!*double_quote && str[*i] == '\'')
		*in_quote = !*in_quote;
	else if (!*in_quote && str[*i] == '\"')
		*double_quote = !*double_quote;
	if (!*in_quote && str[*i] == '\"')
		(*i)++;
	else if (!*double_quote && str[*i] == '\'')
		(*i)++;
}

static char	*extract_filename(char *cmd, int *i)
{
	int		j;
	bool	in_quote = false, double_quote = false;
	char	*file_name;

	file_name = malloc(sizeof(char) * (strlen(cmd) + 1));
	if (!file_name)
		return (NULL);
	j = 0;
	while (cmd[*i] && !(cmd[*i] == ' ' && !in_quote && !double_quote))
	{
		handle_quotes(cmd, i, &in_quote, &double_quote);
		if (cmd[*i] == '>' || cmd[*i] == '<' || (cmd[*i] == ' ' && !in_quote && !double_quote))
			break;
		file_name[j++] = cmd[(*i)++];
	}
	file_name[j] = '\0';
	return (file_name);
}

static char	*update_command(char *cmd, int i)
{
	char	*new_cmd = strdup(&cmd[i]);
	if (!new_cmd)
		return (NULL);
	free(cmd);
	return (new_cmd);
}

char	*epur_filename_deux(t_token *token_head)
{
	char	*file_name;
	int		i = 0;

	// printf("command: %s\n", token_head->command_two);
	skip_until_sequence(token_head->command_two, &i, "<<");
	i += 2; // Skip the "<<"
	while (token_head->command_two[i] == ' ') i++; // Skip spaces after "<<"
	file_name = extract_filename(token_head->command_two, &i);
	if (!file_name)
		return (NULL);
	token_head->command_two = update_command(token_head->command_two, i);
	if (!token_head->command_two)
	{
		free(file_name);
		return (NULL);
	}
	return (file_name);
}

t_token	*handle_multiple_heredocs(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	// int current_position = 0;
	while (token && ft_strcmp_minishell(token->split_value, "<<") == 0)
	{
		delimiter = epur_filename_deux(current->token_head);
		// delimiter = extract_filename_heredoc(current->command, &current_position);
		
		// delimiter = extract_filename_heredoc(token->next->split_value);

		// if (current->heredoc) // malloc(): unaligned fastbin chunk detected 3
		// 	free(current->heredoc);
		printf("%sdelimiter = %s | %s", BLUE, delimiter, RESET);
		current->heredoc = create_heredoc();
		printf("%scurrent->heredoc = %s %s\n", MAGENTA, current->heredoc, RESET);
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
		add_to_heredocs_list(current, current->heredoc);
		write_in_fd(fd, delimiter, current);
		// if (delimiter) //NEW
		// 	free(delimiter); //NEW
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
