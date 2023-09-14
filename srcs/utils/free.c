/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/14 14:31:48 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	ft_free_tokens(t_token *head)
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

void	ft_free_current(t_command *current)
{
	t_command *tmp;

	tmp = NULL;
	while (current)
	{
		tmp = current;
		tmp->command_path = NULL;
		current = current->next;
		free(tmp->command_path);
		free(tmp->command);
		free(tmp);
	}
}