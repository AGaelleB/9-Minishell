/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/13 15:37:06 by abonnefo         ###   ########.fr       */
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

void	ft_free_token(t_command *current)
{
	t_token	*head;
	t_token	*tmp;

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
	ft_free_tab(current->export_arg);
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
