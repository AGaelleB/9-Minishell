/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote_export.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:55:59 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 17:45:06 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	skip_paired_quotes(char *str, int *i, bool *single_quote,
	bool *double_quote)
{
	if (!*single_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		*i += 2;
	else if (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
		*i += 2;
}

static void	toggle_quote_flags(char c, bool *single_quote, bool *double_quote)
{
	if (c == '\'')
		*single_quote = !*single_quote;
	else if (c == '\"')
		*double_quote = !*double_quote;
}

static bool	should_skip_char(char c, bool single_quote, bool double_quote)
{
	return ((!single_quote && c == '\"') || (!double_quote && c == '\''));
}

char	*handle_quotes_export(char *str)
{
	int		i;
	int		y;
	bool	single_quote;
	bool	double_quote;
	char	*dest;

	i = 0;
	y = 0;
	single_quote = false;
	double_quote = false;
	dest = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!dest)
		return (NULL);
	while (str[i])
	{
		skip_paired_quotes(str, &i, &single_quote, &double_quote);
		if (str[i] == '\0')
			break ;
		toggle_quote_flags(str[i], &single_quote, &double_quote);
		if (should_skip_char(str[i], single_quote, double_quote))
			i++;
		else
			dest[y++] = str[i++];
	}
	return (dest[y] = '\0', dest);
}
