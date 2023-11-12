/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/12 13:03:58 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;
	int	status;

	i = -1;
	signal(SIGINT, SIG_IGN);
	while (++i <= command->nb_pipes)
	{
		if (waitpid(child_pids[i], &status, 0) > 0)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = WTERMSIG(status) + 128;
		}
	}
	signal(SIGINT, ctrl_c_main);
}

// par contre je ne close surement pas bien mes fd : close(data->heredocs[i].fd[0]);
void	ctrl_c_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		g_exit_status = 130;
		write(2, "\n", 1); // si coms n affiche plus C^ sur le readline
		exit(g_exit_status); 
	}
}

int	here_doc_manage(t_process_data *data, int fd[2], char *delimiter)
{
	char	*input;
	int		i;

	i = 0;
	(void)data;
	signal(SIGINT, ctrl_c_heredoc);
	close(fd[0]);
	while (1)
	{
		input = readline("> ");
		if (ctrl_d_heredoc(input, i, delimiter) == 45)
		{
			free(input);
			return (45);
		}
		if (ft_strcmp_minishell(input, delimiter) == 0)
		{
			free(input);
			close(fd[1]);
			exit (1);
		}
		write(fd[1], input, ft_strlen(input));
		write(fd[1], "\n", 1);
		free(input);
		i++;
	}
		signal(SIGQUIT, ctrl_c_main);
	return (0);
}

int	here_doc_rayan(t_process_data *data)
{
	t_token *redir;
	int		i;
	int		pid;
	int		status;

	data->count_hd = 0;
	i = 0;
	redir = data->current->token_head;
	while (redir)
	{
		if (redir->type == TYPE_HEREDOC)
			data->count_hd++;
		redir = redir->next;
	}
	data->heredocs = malloc(sizeof(t_here_doc) * data->count_hd);
	redir = data->current->token_head;
	signal(SIGINT, ctrl_c_heredoc);
	while (i < data->count_hd)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			// Si un heredoc a été interrompu, arrêtez d'attendre les autres
			//est ce que je pourrais pas close ici ?? 
			close(data->heredocs[i].fd[0]);
			close(data->heredocs[i].fd[1]);
			i++;
			break;
		}
		pipe(data->heredocs[i].fd);
		pid = fork();
		char	*delimiter = epur_filename_heredoc(data->current->token_head);
		if (pid == 0)
		{
			here_doc_manage(data, data->heredocs[i].fd, delimiter);
			exit(1);
		}
		waitpid(pid, NULL, 0);
		close(data->heredocs[i].fd[1]);
		i++;
	}
	signal(SIGQUIT, ctrl_c_main);
	return (0);
}

static void	handle_execve_processes(t_process_data *data, t_env *env)
{
	while (data->current)
	{
		if (pipe(data->current->fd) == -1)
			exit_with_error("pipe", data->child_pids);
		here_doc_rayan(data);
		data->pid = fork();
		data->child_pids[data->index++] = data->pid;
		data->current->fd_in = data->current->fd[0];
		data->current->fd_out = data->current->fd[1];
		handle_all_process(data, env);
		data->current = data->current->next;
	}
}

void	execve_fd(t_command *current, t_env *env)
{
	t_process_data	data;

	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	data.current_hd = 0;
	init_execve(current, &(data.child_pids));
	data.current->flag = 0;
	data.current = current;
	handle_execve_processes(&data, env);
	wait_for_children(data.command, data.child_pids);
	cleanup(data.child_pids, data.infile);
}
