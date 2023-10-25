/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 09:56:01 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/25 10:05:59 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_realloc(void *ptr, size_t original_size, size_t new_size)
{
	size_t	size_to_copy;
	void	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (original_size < new_size)
		size_to_copy = original_size;
	else
		size_to_copy = new_size;
	ft_memcpy(new_ptr, ptr, size_to_copy);
	free(ptr);
	return (new_ptr);
}
