/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epur_heredoc_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:04:43 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 15:08:30 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	skip_until_sequence(char *str, int *i, char *redir)
{
	while (str[*i] && (str[*i] != redir[0]
			|| ft_strncmp(&str[*i], redir, ft_strlen(redir)) != 0))
		(*i)++;
}

static char	*extract_filename(char *cmd, int *i)
{
	int		j;
	bool	single_quote;
	bool	double_quote;
	char	*file_name;

	single_quote = false;
	double_quote = false;
	file_name = malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!file_name)
		return (NULL);
	j = 0;
	while (cmd[*i] && !(cmd[*i] == ' ' && !single_quote && !double_quote))
	{
		handle_quotes_master(cmd, i, &single_quote, &double_quote);
		if (cmd[*i] == '>' || cmd[*i] == '<'
			|| (cmd[*i] == ' ' && !single_quote && !double_quote))
			break ;
		file_name[j++] = cmd[(*i)++];
	}
	file_name[j] = '\0';
	return (file_name);
}

static char	*update_command(char *cmd, int i)
{
	char	*new_cmd;

	new_cmd = strdup(&cmd[i]);
	if (!new_cmd)
		return (NULL);
	free(cmd);
	return (new_cmd);
}

char	*epur_filename_heredoc(t_token *token_head)
{
	char	*file_name;
	int		i;

	i = 0;
	skip_until_sequence(token_head->command_two, &i, "<<");
	i += 2;
	while (token_head->command_two[i] == ' ')
		i++;
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
