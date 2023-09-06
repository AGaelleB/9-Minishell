/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 14:05:06 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/06 14:56:02 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	**split_string(const char *str, char delimiter)
// {
// 	int i;
// 	int j;
// 	int k;
// 	int token_count;
// 	char **tokens;
// 	int start;
// 	int token_size;

// 	token_count = 1;
// 	i = 0;
// 	while(str[i])
// 	{
// 		if (str[i] == delimiter)
// 			token_count++;
// 		i++;
// 	}

// 	tokens = malloc((token_count + 1) * sizeof(char *));
// 	if (!tokens)
// 	{
// 		perror("malloc");
// 		return (NULL);
// 	}

// 	start = 0;
// 	i = 0;
// 	j = 0;

// 	while(i <= token_count)
// 	{
// 		if (str[j] == '\0' || str[j] == delimiter)
// 		{
// 			token_size = j - start + 1;
// 			tokens[i] = malloc(token_size);
// 			if (!tokens[i])
// 			{
// 				perror("malloc");
// 				k = 0;
// 				while(k < i)
// 				{
// 					free(tokens[k]);
// 					k++;
// 				}
// 				free(tokens);
// 				return (NULL);
// 			}
// 			strncpy(tokens[i], &str[start], token_size - 1);
// 			tokens[i][token_size - 1] = '\0';
// 			start = j + 1;
// 		}
// 		j++;
// 		i++;
// 	}
// 	tokens[token_count] = NULL; // Marquer la fin du tableau avec NULL
// 	return (tokens);
// }

/////////////

char	**split_string(const char *str, char delimiter)
{
	int i;
	int j;
	int k;
	int token_count;
	char **tokens;
	int start;
	int token_size;

	token_count = 1;
	i = 0;
	while(str[i])
	{
		if (str[i] == delimiter)
			token_count++;
		i++;
	}

	tokens = malloc((token_count + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("malloc");
		return NULL;
	}

	start = 0;
	i = 0;
	j = 0;

	while(str[j])
	{
		if (str[j] == delimiter || str[j + 1] == '\0')
		{
			if (str[j] == delimiter)
				token_size = j - start;
			else
				token_size = j - start + 1;
			tokens[i] = malloc(token_size + 1);
			if (!tokens[i])
			{
				perror("malloc");
				k = 0;
				while(k < i)
				{
					free(tokens[k]);
					k++;
				}
				free(tokens);
				return (NULL);
			}
			strncpy(tokens[i], &str[start], token_size);
			tokens[i][token_size] = '\0';
			start = j + 1;
			i++;
		}
		j++;
	}
	tokens[i] = NULL; // Marquer la fin du tableau avec NULL
	return (tokens);
}
