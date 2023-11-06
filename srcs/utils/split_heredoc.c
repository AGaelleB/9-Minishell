/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 11:45:54 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 12:16:39 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static bool	ft_isquote(char c)
{
	return (c == '"' || c == '\'');
}

static int	ft_wordlen(char *s, char c)
{
	int	len = 0;
	bool	in_quote = false;
	char	current_quote = 0;

	while (s[len] && (in_quote || s[len] != c))
	{
		if (ft_isquote(s[len]))
		{
			if (!in_quote)
			{
				in_quote = true;
				current_quote = s[len];
			}
			else if (current_quote == s[len])
			{
				in_quote = false;
			}
		}
		len++;
	}
	return (len);
}

static unsigned int	ft_countwords(char *s, char c)
{
	int		count;
	bool	in_quote;
	char	current_quote;

	count = 0;
	in_quote = false;
	while (*s)
	{
		while (*s == c && !in_quote)
			s++;
		if (*s != '\0')
			count++;
		while (*s && (in_quote || *s != c))
		{
			if (ft_isquote(*s))
			{
				if (!in_quote)
				{
					in_quote = true;
					current_quote = *s;
				}
				else if (current_quote == *s)
				{
					in_quote = false;
				}
			}
			s++;
		}
	}
	return (count);
}

static void	ft_copyword(char *dst, char *src, char c)
{
	bool	in_quote = false;
	char	current_quote = 0;

	while (*src && (in_quote || *src != c))
	{
		if (ft_isquote(*src))
		{
			if (!in_quote)
			{
				in_quote = true;
				current_quote = *src;
			}
			else if (current_quote == *src)
			{
				in_quote = false;
			}
		}
		*dst++ = *src++;
	}
	*dst = '\0';
}

char	**ft_split_heredoc(char *s, char c)
{
	char			**tab;
	unsigned int	nb_words;
	unsigned int	i;
	int				len;

	if (s == 0)
		return (NULL);
	nb_words = ft_countwords(s, c);
	tab = malloc((nb_words + 1) * sizeof(char *));
	if (tab == NULL)
		return (NULL);
	i = 0;
	while (i < nb_words)
	{
		while (*s == c)
			s++;
		len = ft_wordlen(s, c);
		tab[i] = malloc((len + 1) * sizeof(char));
		if (tab[i] == NULL)
		{
			while (i > 0)
				free(tab[--i]);
			free(tab);
			return (NULL);
		}
		ft_copyword(tab[i], s, c);
		s += len;
		i++;
	}
	tab[nb_words] = NULL;
	return (tab);
}
