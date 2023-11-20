/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:39:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 14:51:46 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes_master(char *str, int *i, bool *single_quote,
	bool *double_quote)
{
	if (str[*i] == '\'' && !*double_quote && str[*i + 1] != '\'')
	{
		*single_quote = !*single_quote;
		(*i)++;
	}
	else if (str[*i] == '\"' && !*single_quote && str[*i + 1] != '\"')
	{
		*double_quote = !*double_quote;
		(*i)++;
	}
	while (!*single_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		*i += 2;
	while (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
		*i += 2;
	if (!*double_quote && str[*i] == '\'')
		*single_quote = !*single_quote;
	else if (!*single_quote && str[*i] == '\"')
		*double_quote = !*double_quote;
	if (!*single_quote && str[*i] == '\"')
		(*i)++;
	else if (!*double_quote && str[*i] == '\'')
		(*i)++;
}

char	*epurstr(char *str)
{
	int		y;
	int		i;
	char	*ret;
	bool	double_quote;
	bool	single_quote;

	i = 0;
	y = 0;
	double_quote = false;
	single_quote = false;
	ret = malloc(sizeof(char) * ft_strlen(str));
	while (str[i])
	{
		handle_quotes_master(str, &i, &single_quote, &double_quote);
		ret[y++] = str[i++];
	}
	ret[y] = '\0';
	return (ret);
}
