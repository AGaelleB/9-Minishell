/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 17:18:18 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/16 14:23:12 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
Alloue (avec malloc(3)) et retourne une nouvelle
chaîne, résultat de la concaténation de s1 et s2.
*/

#include "../includes/libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	size_s1;
	size_t	size_s2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	size_s1 = ft_strlen(s1);
	size_s2 = ft_strlen(s2);
	str = malloc(sizeof(*str) * (size_s1 + size_s2 + 1));
	if (str == 0)
		return (NULL);
	ft_strlcpy(str, s1, (size_s1 + 1));
	ft_strlcat(str, s2, (size_s1 + size_s2 + 1));
	return (str);
}
