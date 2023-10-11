/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   epur_filename.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 17:04:47 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/11 17:38:54 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	skip_until_char(char *str, int *i, char c)
{
	while (str[*i] != c)
		(*i)++;
}

static void	handle_quotes(char *str, int *i, bool *in_quote, bool *double_quote)
{
	if (str[*i] == '\'')
		*in_quote = !*in_quote;
	if (str[*i] == '\"')
		*double_quote = !*double_quote;
	if (!*in_quote && str[*i] == '\"')
		(*i)++;
	if (!*double_quote && str[*i] == '\'')
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
	file_name = malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!file_name)
		return (NULL);
	j = 0;
	while (cmd[*i])
	{
		if (cmd[*i] == '>' || cmd[*i] == '<')
			break ;
		handle_quotes(cmd, i, &in_quote, &double_quote);
		if (!in_quote && !double_quote)
			break ;
		file_name[j++] = cmd[(*i)++];
	}
	file_name[j] = '\0';
	return (file_name);
}

/* static char	*update_command(char *cmd, int i)
{
	int		j;
	char	*tempo;

	tempo = malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!tempo)
		return (NULL);
	j = 0;
	while (cmd[i])
		tempo[j++] = cmd[i++];
	tempo[j] = '\0';
	return (tempo);
} */

static char	*update_command(char *cmd, int *i)
{
	int		j;
	int		k;
	char	*tempo;

	k = *i;
	tempo = malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!tempo)
		return (NULL);
	j = 0;
	while (cmd[*i])
		tempo[j++] = cmd[(*i)++];
	tempo[j] = '\0';
	*i = k;
	return (tempo);
}

char	*epur_filename(t_token *token_head)
{
	char	*file_name;
	int		i;

	i = 0;
	skip_until_char(token_head->command, &i, '>');
	while ((token_head->command[i] != '\'') && (token_head->command[i] != '\"'))
		i++;
	file_name = extract_filename(token_head->command, &i);
	if (!file_name)
		return (NULL);
	token_head->command = update_command(token_head->command, &i);
	if (!token_head->command)
	{
		free(file_name);
		return (NULL);
	}
	return (file_name);
}