/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 16:47:55 by abonnefo         ###   ########.fr       */
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

void	ft_free_herdocs(t_command *current)
{
	int	i;

	i = 0;
	while (current->heredocs && current->heredocs[i])
	{
		free(current->heredoc);
		free(current->heredocs[i]);
		i++;
	}
	free(current->heredocs);
}

void ft_free_token(t_command *current)
{
	t_token *head;
	t_token *tmp;

	head = NULL;
	tmp = NULL;
	// printf("%sI'M FREE TOKEN\n%s", YELLOW, RESET);
	while (current)
	{
		free(current->token_head->command);
		free(current->token_head->command_two);
		head = current->token_head;
		while (head)
		{
			tmp = head;
			head = head->next;
			free(tmp->split_value);
			free(tmp);
		}
		current->token_head = NULL;
		current = current->next;
	}
}

void	ft_free_current(t_command *current)
{
	t_command	*tmp;

	tmp = NULL;
	// printf("%sI WILL COMMAND\n%s", GREEN, RESET);
	ft_free_herdocs(current);
	ft_free_tab(current->command_arg);
	while (current)
	{
		tmp = current;
		current = current->next;
		tmp->command_path = NULL;
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
		i++;
	}
	cur->heredocs = NULL;
}

void	cleanup(pid_t *child_pids, int infile)
{
	free(child_pids);
	if (infile != 0)
		close(infile);
}

/* void	ft_free_token(t_command *current, t_token *head)
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
				// head->command = NULL;
				// if (tmp->command != NULL)
				// 	free(tmp->command);
				free(tmp->split_value);
				// free(tmp);
				head = head->next;
			}
			free(current->token_head->command);
			if(current->token_head != NULL)
				free(current->token_head);
			// if(head != NULL) //visiblement inutile
			// 	free(head);
		}
		current = current->next;
	}
}
 */