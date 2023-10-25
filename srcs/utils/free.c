/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/25 17:43:54 by abonnefo         ###   ########.fr       */
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

void	ft_free_struct(t_command *current, t_token *head)
{
	t_token	*tmp;

	tmp = NULL;
	while (current)
	{
		head = current->token_head;
		if (head != NULL)
		{
			while (head)
			{
				tmp = head;
				free(tmp->split_value);
				// if(tmp != NULL) Problme de l'invalid read size ??
				free(tmp); //ICI
				head = head->next;
			}
		}
		current = current->next;
	}
}

void	ft_free_current(t_command *current)
{
	t_command	*tmp;

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

void	clean_heredoc_files(t_command *cur)
{
	int	i;

	i = 0;
	while (cur->heredocs && cur->heredocs[i])
	{
		unlink(cur->heredocs[i]);
		// free(cur->heredocs[i]); // casse l exec des pipe
		i++;
	}
	// free(cur->heredocs); // peut faire des doubles free sur une cmd basic avec pipe
	cur->heredocs = NULL;
}

void	cleanup(pid_t *child_pids, int infile)
{
	free(child_pids);
	if (infile != 0)
		close(infile);
}
