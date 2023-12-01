/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/01 11:10:53 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_env(t_env *env)
{
	int	i;

	i = 0;
	// printf("%sI'M FREE ENV\n%s", BLUE, RESET);
	while (env->cpy_env[i])
	{
		free(env->cpy_env[i]);
		i++;
	}
	free(env->cpy_env);
	free(env);
}

void	free_export(t_export *export)
{
	// if (export->ret)
	// 	free(export->ret); // nous fait perdre 6 tests
	if (export->new)
		free(export->new);
	if (export)
		free(export);
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
		if (current->token_head->command)
			free(current->token_head->command);
		if (current->token_head->command_two)
		free(current->token_head->command_two);
		head = current->token_head;
		while (head)
		{
			tmp = head;
			head = head->next;
			if (tmp->split_value)
				free(tmp->split_value);
			if (tmp)
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
	// printf("%sI'M FREE COMMAND\n%s", GREEN, RESET);
	ft_free_herdocs(current);
	
	// if (current->command_arg_main) // invalid free
	// 		ft_free_tab(current->command_arg_main);
	if (current->command_arg)
		ft_free_tab(current->command_arg);
	if (current->export_arg)
		ft_free_tab(current->export_arg);
	while (current)
	{
		tmp = current;
		current = current->next;
		tmp->command_path = NULL;
		if (tmp->command_path)
			free(tmp->command_path);
		if (tmp->command)
			free(tmp->command);
		if (tmp)
			free(tmp);
	}
}
