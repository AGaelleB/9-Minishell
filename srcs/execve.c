/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/11 16:56:59 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// t_command	*get_command(char *input)
// {
// 	t_command	*head;
// 	t_command	*current;
// 	t_command	*new_cmd;
// 	char		**command;
// 	int			i;

// 	head = NULL;
// 	current = NULL;
// 	command = split_string(input, '|');
// 	i = 0;
// 	while(command[i])
// 	{
// 		new_cmd = malloc(sizeof(t_command));
// 		new_cmd->command = ft_strdup(command[i]);
// 		if (!head)
// 		{
// 			head = new_cmd;
// 			current = head;
// 		}
// 		else
// 		{
// 			current->next = new_cmd;
// 			current = new_cmd;
// 		}
// 		i++;
// 	}
// 	return (current);
// }


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
			// Gérer l'erreur, peut-être sortir du programme ou retourner NULL
			perror("Failed to allocate memory for new command");
			exit(1);
		}
		new_cmd->command = ft_strdup(command[i]);
		if (!new_cmd->command)
		{
			// Gérer l'erreur
			perror("Failed to duplicate command string");
			exit(1);
		}
		new_cmd->next = NULL;  // Initialise next à NULL
		
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
	// Libérer la mémoire pour le tableau command si nécessaire
	// free(command);

	return (head);  // Retourner la tête de la liste
}


void	ft_set_args_and_paths(t_command *current, char **envp)
{
	// printf("debutt set path and argc\n\n");
	current->command_arg = ft_split(current->command, ' ');
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
	// printf("PATH : %s\n\n", current->command_path);

}

void child_process(t_command *current, int read_fd, int write_fd, char **envp)
{
	// printf("Début child process\n");

	dup2(read_fd, 0);  // Duplique read_fd vers stdin
	if (write_fd != 1)
		dup2(write_fd, 1);  // Duplique write_fd vers stdout
   
	ft_set_args_and_paths(current, envp);  // Met à jour args et paths
	
	if (current->command_path == NULL)
	{
		write(2, "command not found: ", 19);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, "\n", 1);
		exit(127);  // Code de sortie standard pour "commande introuvable"
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1) 
	{
		perror("Error");
		exit(-1);
	}
}
