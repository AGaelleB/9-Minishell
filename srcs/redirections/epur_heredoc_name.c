/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epur_heredoc_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:04:43 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/12 10:59:51 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	skip_until_sequence(char *str, int *i, char *redir)
{
	while (str[*i] && (str[*i] != redir[0]
			|| ft_strncmp(&str[*i], redir, ft_strlen(redir)) != 0))
		(*i)++;
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
	bool	in_quote;
	bool	double_quote;
	char	*file_name;

	in_quote = false;
	double_quote = false;
	file_name = malloc(sizeof(char) * (strlen(cmd) + 1));
	if (!file_name)
		return (NULL);
	j = 0;
	while (cmd[*i] && !(cmd[*i] == ' ' && !in_quote && !double_quote))
	{
		handle_quotes(cmd, i, &in_quote, &double_quote);
		if (cmd[*i] == '>' || cmd[*i] == '<'
			|| (cmd[*i] == ' ' && !in_quote && !double_quote))
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
	// printf("%scommand before : %s\n%s", YELLOW, token_head->command_two, RESET);
	token_head->command_two = update_command(token_head->command_two, i);
	// printf("%scommand after : %s\n%s", MAGENTA, token_head->command_two, RESET);
	if (!token_head->command_two)
	{
		free(file_name);
		return (NULL);
	}
	return (file_name);
}
