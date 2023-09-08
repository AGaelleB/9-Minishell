/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/08 16:48:35 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		new_cmd->command = ft_strdup(command[i]);
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
	return (current);
}

void	ft_set_args_and_paths(t_command *current, char **envp)
{
	current->command_arg = ft_split(current->command, ' ');
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
}

void child_proces(t_command *current, int read_fd, int write_fd, char **envp)
{
	dup2(read_fd, 0);  // Duplique read_fd vers stdin
	if (write_fd != 1)
		dup2(write_fd, 1);  // Duplique write_fd vers stdout
	close(read_fd);  // Ferme les FD dupliqués
	close(write_fd);

	ft_set_args_and_paths(current, envp);  // Met à jour args et paths

	if (current->command_path == NULL) {
		write(2, "command not found: ", 19);
		write(2, current->command_arg[0], strlen(current->command_arg[0]));
		write(2, "\n", 1);
		exit(127);  // Code de sortie standard pour "commande introuvable"
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1)
	{
		perror("Error");
		exit(-1);
	}
}


void	exec_pipe(t_command *current, int read_fd, int write_fd, char **envp)
{
	int pid;
	
	pid = fork();
	if (pid == 0)
		child_proces(current, read_fd, write_fd, envp);
	waitpid(pid, NULL, 0);
}
