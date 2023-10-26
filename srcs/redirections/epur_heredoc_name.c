/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epur_heredoc_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 10:04:43 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/26 11:42:02 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes_heredoc(char *str, int *i,
	bool *in_quote, bool *double_quote)
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

static int	skip_to_heredoc(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != '<')
		i++;
	while (cmd[i] == '<' || cmd[i] == ' ')
		i++;
	return (i);
}

char	*extract_filename_heredoc(char *cmd)
{
	int		j;
	bool	in_quote;
	bool	double_quote;
	char	*file_name;
	int		i;

	in_quote = false;
	double_quote = false;
	file_name = malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!file_name)
		return (NULL);
	i = skip_to_heredoc(cmd);
	j = 0;
	while (cmd[i])
	{
		handle_quotes_heredoc(cmd, &i, &in_quote, &double_quote);
		if (!in_quote && !double_quote && cmd[i] == ' ')
			break ;
		file_name[j++] = cmd[i++];
	}
	file_name[j] = '\0';
	return (file_name);
}

