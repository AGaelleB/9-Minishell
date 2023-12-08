/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_expr_dollar.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 10:39:08 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/08 13:59:15 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*copy_exit_status_to_temp(char *temp, int *j, int *i)
{
	char	*temp_exit_status;
	int		k;

	k = 0;
	temp_exit_status = ft_itoa(g_exit_status);
	if (!temp_exit_status)
		return (NULL);
	while (temp_exit_status[k])
	{
		temp[*j] = temp_exit_status[k];
		(*j)++;
		k++;
	}
	free(temp_exit_status);
	(*i) += 2;
	return (temp);
}

char	*replace_exit_status(char *str)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	j = 0;
	temp = malloc(sizeof(char) * SIZE);
	if (!temp)
		return (NULL);
	ft_memset(temp, '\0', SIZE);
	while (str[i])
	{
		if ((str[i] == '$') && (str[i + 1] == '?'))
		{
			temp = copy_exit_status_to_temp(temp, &j, &i);
			if (!temp)
				return (NULL);
			continue ;
		}
		temp[j] = str[i];
		i++;
		j++;
	}
	temp[j] = '\0';
	return (temp);
}

char	*check_expr(char *str)
{
	char	*temp;

	temp = replace_exit_status(str);
	if (!temp)
		return (NULL);
	free(str);
	return (temp);
}
