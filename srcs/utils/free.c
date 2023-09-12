/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/12 13:35:10 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// penser a free les tokens


void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	ft_free_all_cmd(t_command *command)
{
	
	ft_free_tab(command->command_arg); //cat test.txt | wc | rev
}


void free_tokens(t_token *head)
{
	t_token *tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->split_value);
		free(tmp);
	}
}
