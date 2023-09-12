/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/12 12:23:07 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void open_fd(t_command *current, char **envp)
{
	t_command	*command;
	pid_t		pid;
	pid_t		*child_pids;
	int			fd[2];
	int			infile;
	int			num_children;
	int			index;
	int			i;

	command = current;
	infile = 0;
	num_children = 0;
	index = 0;
	i = -1;
	while(current)
	{
		num_children++;
		current = current->next;
	}
	current = command;
	child_pids = malloc(num_children * sizeof(pid_t));
	while (current)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		current->read_fd = infile;
		if (current->next != NULL)
			current->write_fd = fd[1];
		else
			current->write_fd = 1;
		pid = fork();
		child_pids[index] = pid; // Stocker le PID
		index++;
		if (pid == 0)
			child_process(current, current->read_fd, current->write_fd, envp);
		else if (pid > 0)
		{
			close(fd[1]);
			if (current->next != NULL)
				infile = fd[0];
			else
				close(fd[0]);
		}
		else
		{
			perror("fork");
			exit(1);
		}
		current = current->next;
	}
	while (i < command->nb_pipes)
	{
		++i;
		waitpid(child_pids[i], NULL, 0);
	}
	free(child_pids);
}


// RAYAN
// 	int status;
// 	int i = 0;
// 	printf("nombre de pipes final : %d\n", command->nb_pipes);
// 	while (i < command->nb_pipes)
// 	{
// 		// printf("test\n");
// 		waitpid(0, &status, -1);
// 		i++;
// 	}
// 	WEXITSTATUS(status);
// 	(void)token;