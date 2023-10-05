/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 12:38:34 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void execve_fd(t_command *current, char **envp)
{
	t_command	*command;
	pid_t		pid;
	pid_t		*child_pids;
	int			infile;
	int			num_children;
	int			index;
	int			i;

	command = current;
	infile = 0;
	num_children = 0;
	index = 0;
	i = -1;
	while (current)
	{
		num_children++;
		current = current->next;
	}
	current = command;
	child_pids = malloc(num_children * sizeof(pid_t));
	if (!child_pids)
		exit(1);
	while (current)
	{
		if (pipe(current->fd) == -1)
		{
			perror("pipe");
			free(child_pids);
			exit(1);
		}
		pid = fork();
		child_pids[index++] = pid;
		current->fd_in = current->fd[0];
		current->fd_out = current->fd[1];
		if (pid == 0)
		{
			close(current->fd_in);
			dup2(infile, 0);
			if (current->next)
				dup2(current->fd_out, 1);
			close(current->fd_out);
			ft_close_fd();
			if (current->fd_in != -1)
			{
				dup2(current->fd_in, 0);
				close(current->fd_in);
			}
			if (current->fd_out != -1)
			{
				dup2(current->fd_out, 1);
				close(current->fd_out);
			}

			ft_set_args_and_paths(current, envp); // Définir les arguments et le chemin avant d'appeler child_process.
			// print_commands_and_tokens(current); // PRINT
			open_fd(current);
			ft_all_builtins_verif(current);

			if(child_process(current, envp) == 127)
			{
				free(child_pids);
				exit(127);
			}
		}
		else if (pid > 0)
		{
			close(current->fd_out);
			if (infile != 0)
				close(infile);
			infile = current->fd_in;
		}
		else
		{
			perror("fork");
			free(child_pids);
			exit(1);
		}
		current = current->next; 
	}
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		++i;
		waitpid(child_pids[i], NULL, 0);
	}
	signal(SIGINT, ft_signal_ctrl_C);
	free(child_pids);
	if (infile != 0)
		close(infile);
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