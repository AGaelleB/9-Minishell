/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/22 16:40:35 by bfresque         ###   ########.fr       */
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

void	ft_free_struct(t_command *current, t_token *head) //modif ici
{
	t_token *tmp;

	tmp = NULL;
	while (current)
	{
		head = current->token_head;
		if(head != NULL)
		{
			while (head)
			{
				tmp = head;
				free(tmp->split_value);
				// if(tmp != NULL)
					free(tmp); //ICI
				head = head->next;
			}
		}
		current = current->next;
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

void	free_file_name(char *file_name)
{
	if (file_name != NULL)
	{
		unlink(file_name);
		free(file_name);
		ft_close_fd();
	}
}
