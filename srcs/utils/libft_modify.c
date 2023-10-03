/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_modify.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:16:28 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/03 14:15:29 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_strchr_slash(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	ft_strncmp_minishell(char *s1, char *s2, int n)
{
	int	i;

	i = 0;
	while ((i < n) && (s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	if (i < n)
		return (s1[i] - s2[i]);
	return (0);
}

int	ft_strcmp_minishell(char *s1, char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] == s2[i]) && s1[i] && s2[i])
		i++;
	return (s1[i] - s2[i]);
}

char	*ft_strjoin_minishell(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*dest;

	i = ft_strlen(s1) + ft_strlen(s2);
	dest = malloc(sizeof(char) * (i + 2));
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '/';
	i++;
	while (s2[j])
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

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
	tokens[i] = NULL;
	return (tokens);
}


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

char **split_string_token(char *str, char **delimiters)
{
	int i;
	int j;
	int k;
	int token_count = 1; 
	int start;
	int token_size;
	int delim_len = 0;

	// Calculating tokens including delimiters
	i = 0;
	while(str[i])
	{
		if (is_delimiter(&str[i], delimiters, &delim_len))
		{
			token_count += 2; // One for the token before and one for the delimiter
			i += delim_len;   // skip delimiter characters
		}
		else
		{
			i++;
		}
	}

	char **tokens = malloc((token_count + 1) * sizeof(char *));
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
		if (is_delimiter(&str[j], delimiters, &delim_len) || str[j + 1] == '\0')
		{
			if (is_delimiter(&str[j], delimiters, &delim_len))
				token_size = j - start;
			else
				token_size = j - start + 1;

			tokens[i] = malloc(token_size + 1);
			if (!tokens[i])
			{
				perror("malloc");
				for (k = 0; k < i; k++)
					free(tokens[k]);
				free(tokens);
				return NULL;
			}
			strncpy(tokens[i], &str[start], token_size);
			tokens[i][token_size] = '\0';

			i++;

			// Storing delimiter as a token
			if (delim_len > 0)
			{
				tokens[i] = malloc(delim_len + 1);
				if (!tokens[i])
				{
					perror("malloc");
					for (k = 0; k < i; k++)
						free(tokens[k]);
					free(tokens);
					return NULL;
				}
				strncpy(tokens[i], &str[j], delim_len);
				tokens[i][delim_len] = '\0';

				i++;
				j += delim_len; // skip delimiter characters
				start = j;
			}
			else
			{
				j++;
				start = j;
			}
		}
		else
		{
			j++;
		}
	}
	tokens[i] = NULL;
	return (tokens);
}
