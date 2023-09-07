/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/07 11:07:19 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_set_args_and_paths(t_command *current, char **envp)
{
	current->command_arg = ft_split(current->command, ' ');
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
}

void	child_proces(t_command *current, int read_fd, int write_fd, char **envp)
{
	dup2(read_fd, 0);
	if (write_fd != 1) // Si ce n'est pas la sortie standard
		dup2(write_fd, 1);
	close(read_fd);
	close(write_fd);
	ft_set_args_and_paths(current, envp);
	if ((current->command_path != NULL)
		&& (execve(current->command_path, current->command_arg, envp) == -1))
	{
		perror("Error");
		//free
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

void	multiple_pipe(t_command *current, char **envp, int infile, int outfile)
{
	int	fd[2];
	int	read_fd;
	int write_fd;


	read_fd = infile;
	while (current)
	{
		pipe(fd);
		if (current->next)
			write_fd = fd[1];
		else
			write_fd = outfile;
		exec_pipe(current, read_fd, write_fd, envp);
		close(fd[1]);
		read_fd = fd[0];
		current = current->next;
	}
	//free
}
