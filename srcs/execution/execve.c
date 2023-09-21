/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/20 15:39:35 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*get_command(char *input)
{
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;
	char		**command;
	int			i;

	head = NULL;
	current = NULL;
	command = split_string(input, '|');
	i = 0;
	while(command[i])
	{
		new_cmd = malloc(sizeof(t_command));
		if (!new_cmd)
		{
			perror("Failed to allocate memory for new command");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->command = NULL;
		new_cmd->command = ft_strdup(command[i]);
		// printf("command %d: %s\n", i, new_cmd->command);

		new_cmd->token_head = tokenize_input(new_cmd->command); // tokenize la commande actuelle
		// new_cmd->next = NULL;
		if (!new_cmd->command)
		{
			perror("Failed to duplicate command string");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->next = NULL;
		if (!head)
		{
			head = new_cmd;
			current = head;
		}
		else
		{
			current->next = new_cmd;
			current = new_cmd;
		}
		i++;
	}
	ft_free_tab(command);
	return (head);
}

void	ft_set_args_and_paths(t_command *current, char **envp)
{
	current->command_arg = NULL;
	current->command_path = NULL;
	current->command_arg = ft_split(current->command, ' ');
	// printf("arguments 000 : %s\n", current->command_arg[0]);
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
}

int	child_process(t_command *current, char **envp)
{
	ft_set_args_and_paths(current, envp);
	if (current->command_path == NULL)
	{
		write(2, "minishell: command not found: ", 31);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, "\n", 1);
		ft_free_tab(current->command_arg);
		ft_free_current(current);
		return (127);
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1)
	{
		perror("Error");
		exit(-1);
	}
	return (0);
}