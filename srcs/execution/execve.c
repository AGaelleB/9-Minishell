/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/28 16:29:29 by abonnefo         ###   ########.fr       */
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
		new_cmd->token_head = tokenize_input(new_cmd->command);
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
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
}

int	child_process(t_command *current, char **envp)
{
	ft_set_args_and_paths(current, envp);
	// int i = 0;
	// while(current->command_arg[i])
	// {
	// 	printf("child_process - command_arg[%d] = %s\n", i, current->command_arg[i]);
	// 	i++;
	// }
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
	// printf("J'ai execve dans le child_process\n");
	return (0);
}


// int child_process(t_command *current, char **envp)
// {

// 	char *args[] = {"/usr/bin/echo", "1", "2", "3", "4", "5          8", NULL};
// 	if (execve("/usr/bin/echo", args, envp) == -1)

// 	(void)current;
//     // ft_set_args_and_paths(current, envp);
//     // if (current->command_path == NULL)
//     // {
//     //     write(2, "minishell: command not found: ", 31);
//     //     write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
//     //     write(2, "\n", 1);
//     //     ft_free_tab(current->command_arg);
//     //     ft_free_current(current);
//     //     return (127);
//     // }
//     // // Modifiez ici pour créer le bon tableau d'arguments
//     // char *args[] = {current->command_path, NULL}; // Ajoutez plus d'arguments si nécessaire
//     // if (execve(current->command_path, args, envp) == -1)
//     // {
//     //     perror("Error");
//     //     exit(-1);
//     // }
//     // printf("J'ai execve dans le child_process\n");
//     return (0);
// }
