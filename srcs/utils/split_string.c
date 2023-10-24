/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:42:22 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/24 09:50:16 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_tokens(const char *str, char delimiter)
{
	int	i;
	int	token_count;

	i = 0;
	token_count = 1;
	while (str[i])
	{
		if (str[i] == delimiter)
			token_count++;
		i++;
	}
	return (token_count);
}

char	**alloc_tokens(const char *str, char delimiter)
{
	int		token_count;
	char	**tokens;

	token_count = count_tokens(str, delimiter);
	tokens = malloc((token_count + 1) * sizeof(char *));
	if (!tokens)
		perror("malloc");
	return (tokens);
}

char	*allocate_and_copy_token(const char *str, int start, int end)
{
	int		len;
	char	*token;

	len = end - start;
	token = malloc(len + 1);
	if (!token)
	{
		perror("malloc");
		return (NULL);
	}
	strncpy(token, &str[start], len); // FORBIDEN A CODER
	token[len] = '\0';
	return (token);
}

char	**fill_tokens(char **tokens, const char *str, char delimiter)
{
	int	i;
	int	j;
	int	start;
	int	end;

	i = 0;
	j = 0;
	start = 0;
	while (str[j])
	{
		if (str[j] == delimiter || str[j + 1] == '\0')
		{
			if (str[j] == delimiter)
				end = j;
			else
				end = j + 1;
			tokens[i] = allocate_and_copy_token(str, start, end);
			if (!tokens[i])
				return (free_tokens(tokens, i), NULL);
			start = j + 1;
			i++;
		}
		j++;
	}
	return (tokens[i] = NULL, tokens);
}

char	**split_string(const char *str, char delimiter)
{
	char	**tokens;

	tokens = alloc_tokens(str, delimiter);
	if (!tokens)
		return (NULL);
	return (fill_tokens(tokens, str, delimiter));
}
