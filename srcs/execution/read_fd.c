/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/20 17:07:18 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
si < prendre argc passé avant comme infile fd[0]
si > prendre argc passé apres comme outfile fd[1]
*/
int	redirect_file_in(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_in != 0)
		close(current->fd_in);
	filename = token->next->split_value;
	// printf("filemane : %s\n", filename);
	printf("current->fd_in AVANT %d\n", current->fd_in);
	current->fd_in = open(filename, O_RDONLY);
	printf("current->fd_in APRES %d\n", current->fd_in);
	if (current->fd_in == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_file_out(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = token->next->split_value;
	current->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_append_file_out(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = token->next->split_value;
	current->fd_out = open(filename, O_APPEND | O_WRONLY, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	open_fd(t_command *command)
{
	t_token	*token;

	token = command->token_head;
	while (token)
	{
		// printf("open_fd\n");
		if (token->type == TYPE_REDIR_IN)
		{
			// printf("TYPE_REDIR_IN\n");
			if (redirect_file_in(command, token) == 0)
			{
				dup2(command->fd_in, 0);
				close(command->fd_in);
				// printf("fini\n");
			}
		}
		if (token->type == TYPE_REDIR_OUT)
		{
			// printf("TYPE_REDIR_OUT\n");
			if (redirect_file_out(command, token) == 0)
			{
				dup2(command->fd_out, 1);
				close(command->fd_out);
			}
		}
		if (token->type == TYPE_REDIR_APPEND)
		{
			// printf("TYPE_REDIR_OUT\n");
			if (redirect_append_file_out(command, token) == 0)
			{
				dup2(command->fd_out, 1);
				close(command->fd_out);
			}
		}
		token = token->next;
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
			open_fd(current);
			// printf("current->fd_in = %d \n", current->fd_in);
			// printf("current->fd_out = %d \n\n", current->fd_out);
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