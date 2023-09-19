/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/18 16:09:31 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
si < prendre argc passé avant comme infile fd[0]
si > prendre argc passé apres comme outfile fd[1]
*/

int	redirect_file_out(t_command **cmd, t_token *cur)
{
	char	*filename;

	if ((*cmd)->fd_out != 1)
		close((*cmd)->fd_out);
	filename = cur->next->split_value;
	// printf("%s\n", filename); //printf
	(*cmd)->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	// printf("(*cmd)->fd_out : %d\n", (*cmd)->fd_out); //printf
	if ((*cmd)->fd_out == -1)
	{
		perror(filename);
		return (-1);
	}
	return (0);
}

int	open_fd(t_command **cmdl)
{
	t_token	*cur;

	cur = (*cmdl)->token;
	(*cmdl)->fd_out = -1;
	while (cur)
	{
		if (cur->type == TYPE_REDIR_OUT)
		{
			// if (redirect_file_out(cmdl, cur, cur->type) == -1)
			if (redirect_file_out(cmdl, cur) == -1)
				return (-1);
		}
		cur = cur->next;
	}
	return (0);
}

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
	open_fd(&current);
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
		if (pid == 0) // Child
		{
			close(current->fd[0]);
			dup2(infile, 0);
			if (current->next)
				dup2(current->fd[1], 1);
			close(current->fd[1]);
			close_fd();
			if(child_process(current, envp) == 127)
			{
				free(child_pids);
				exit(127);
			}
		}
		else if (pid > 0) // Parent
		{
			close(current->fd[1]);
			if(current->fd_out)
				close(current->fd_out);
			if (infile != 0)
				close(infile);
			infile = current->fd[0];
			
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