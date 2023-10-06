/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:17:48 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/06 15:09:57 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*create_new_cmd(char *command_str, char **envp)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
	{
		perror("Failed to allocate memory for new command");
		exit(1);
	}
	new_cmd->command = add_spaces_around_redirections(command_str);
	new_cmd->token_head = tokenize_input(new_cmd->command, envp);
	if (!new_cmd->command)
	{
		perror("Failed to duplicate command string");
		exit(1);
	}
	new_cmd->next = NULL;
	return (new_cmd);
}

t_command	*append_new_cmd(t_command **head, t_command *new_cmd)
{
	t_command	*current;

	if (!(*head))
	{
		*head = new_cmd;
		current = *head;
	}
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_cmd;
	}
	return (current);
}

t_command	*get_command(char *input, char **envp)
{
	t_command	*head;
	t_command	*new_cmd;
	char		**command;
	int			i;

	head = NULL;
	command = split_string(input, '|');
	i = 0;
	while (command[i])
	{
		new_cmd = create_new_cmd(command[i], envp);
		append_new_cmd(&head, new_cmd);
		i++;
	}
	ft_free_tab(command);
	return (head);
}
