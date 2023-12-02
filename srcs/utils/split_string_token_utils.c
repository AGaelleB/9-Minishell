/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   .c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:46:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/06 17:46:03 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_delimiter(char *str, char **delimiters, int *delim_len)
{
	int	i;

	i = 0;
	while (delimiters[i] != NULL)
	{
		if (ft_strncmp(str, delimiters[i], ft_strlen(delimiters[i])) == 0)
		{
			*delim_len = ft_strlen(delimiters[i]);
			return (1);
		}
		i++;
	}
	*delim_len = 0;
	return (0);
}

int	count_split_tokens_str(char *str, char **delimiters)
{
	int	i;
	int	token_count;
	int	delim_len;

	i = 0;
	token_count = 1;
	delim_len = 0;
	while (str[i])
	{
		if (is_delimiter(&str[i], delimiters, &delim_len))
		{
			token_count += 2;
			i += delim_len;
		}
		else
			i++;
	}
	return (token_count);
}

char	**allocate_tokens(int token_count)
{
	char	**tokens;

	tokens = malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	return (tokens);
}

char	*allocate_and_cpy(char *start, int size)
{
	char	*new_str;
	int		i;

	new_str = malloc(sizeof(char *) * (size + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_str[i] = start[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
