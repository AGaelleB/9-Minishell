/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:16:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/28 12:22:14 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Alloue (avec malloc(3)) et retourne un tableau
de chaînes de caractères obtenu en séparant ’s’ à
l’aide du caractère ’c’, utilisé comme délimiteur.
Le tableau doit être terminé par NULL.
*/

#include "../includes/libft.h"

static char	ft_separateur(char cmp, char c)
{
	if (cmp == c)
		return (1);
	return (0);
}

static int	ft_alloctxt(char **tab, const char *s, char c)
{
	int	word_len;
	int	i;

	i = 0;
	while (*s)
	{
		while (*s && ft_separateur(*s, c) == 1)
			s++;
		word_len = 0;
		while (*s && ft_separateur(*s, c) == 0)
		{
			word_len++;
			s++;
		}
		if (word_len != 0)
		{
			tab[i] = malloc(word_len + 1);
			if (tab[i] == 0)
				return (0);
			tab[i++][word_len] = 0;
		}
	}
	return (1);
}

static void	ft_filltab(char **tab, const char *s, char c)
{
	int	i;
	int	j;

	i = 0;
	while (*s && tab[i])
	{
		if (*s == '<')  // si on trouve '<', ignorer ce caractère et l'argument suivant
		{
			s++;  // ignorer '<'
			while (*s && *s != ' ' && ft_separateur(*s, c) == 0) // ignorer l'argument suivant
				s++;
		}
		
		j = 0;
		while (*s && ft_separateur(*s, c) == 1)  // ignorer les séparateurs
			s++;
		while (*s && ft_separateur(*s, c) == 0)
		{
			tab[i][j++] = *(s++);
		}
		i++;
	}
}

static unsigned int	ft_countwords(char const *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] && ft_separateur(s[i], c) == 0)
		{
			count++;
			i++;
		}
		while (s[i] && ft_separateur(s[i], c) == 0)
			i++;
		while (s[i] && ft_separateur(s[i], c) == 1)
			i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char			**tab;
	char			*stopChar;
	unsigned int	nb_words;
	unsigned int	i;

	if (s == 0)
		return (NULL);

	// Vérifiez si le caractère '>' est présent
	stopChar = ft_strchr(s, '>');
	if (stopChar != NULL)
		*stopChar = '\0';  // Remplacez temporairement par une fin de chaîne
	
	
	stopChar = ft_strchr(s, '<');
	if (stopChar != NULL)
		*stopChar = '\0';  // Remplacez temporairement par une fin de chaîne

	// if (ft_strchr(s, '<') != NULL) // si on trouve '<', ignorer ce caractère et l'argument suivant
	// {
	// 	i++; // ignorer '<'
	// 	if(s[i] == ' ')
	// 		i++;
	// 	while (s[i] && s[i] != ' ' && ft_separateur(s[i], c) == 0) // ignorer l'argument suivant
	// 		i++;
	// }
	nb_words = ft_countwords(s, c);
	tab = malloc((nb_words + 1) * sizeof(char *));
	if (tab == 0)
		return (0);

	tab[nb_words] = 0;
	if (nb_words > 0)
	{
		if (ft_alloctxt(tab, s, c) == 0)
		{
			i = 0;
			while (tab[i])
				free(tab[i++]);
			free(tab);
			return (0);
		}
		ft_filltab(tab, s, c);
	}
	// Restaurez le caractère '>' si nécessaire
	if (stopChar != NULL)
		*stopChar = '>';
	if (stopChar != NULL)
		*stopChar = '<';

	return (tab);
}

