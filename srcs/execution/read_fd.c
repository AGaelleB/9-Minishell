/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/11 18:45:25 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;
	int	status;

	i = -1;
	// i = 0;
	signal(SIGINT, SIG_IGN);
	// printf("%scommand->nb_pipes = %d %s\n", YELLOW, command->nb_pipes, RESET); // PRINT
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
	signal(SIGINT, ft_builtin_ctrl_c);
	// printf("%swaut children fini %s\n", GREEN, RESET); // PRINT
}

// void	sig_hd(int signal, int pos)
// {
// 	t_process_data	*data;
// 	int			i;
// 	int			hdc;

// 	if (signal == SIGINT)
// 	{
// 		i = 0;
// 		while (i < hdc)
// 		{
// 			if (i == 0)
// 				break ;
// 			close(data->heredocs[i].fd[0]);
// 			close(data->heredocs[i].fd[1]);
// 			i++;
// 		}
// 		// free(data->here_doc);
// 		// free_tab(data->cmd_paths);
// 		// free_kill(data);
// 		// reset_stuff(data);
// 		exit (42);
// 	}
// }

static void	handle_heredoc_signals(t_process_data *data, char *line)
{
	if (g_exit_status == 130)
	{
		int i = 0;
		while (i < data->count_hd)
		{
			if (i == 0)
				break ;
			close(data->heredocs[i].fd[0]);
			close(data->heredocs[i].fd[1]);
			i++;
		}
		clean_heredoc_files(data->current);
		free(line);
		exit(130);
	}
}

int	here_doc_manage(t_process_data *data, int fd[2])
{
	char	*input;
	int i;
	char	*delimiter = epur_filename_heredoc(data->current->token_head);
	i = 0;
	close(fd[0]);
	handle_signals_heredoc();
	while (1)
	{
		input = readline("> ");
		handle_heredoc_signals(data, input);
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
	return (0);
}

int	here_doc_rayan(t_process_data *data)
{
	t_token *redir;
	int		i;
	int		pid;

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
	while (i < data->count_hd)
	{
		pipe(data->heredocs[i].fd);
		pid = fork();
		signal(SIGINT, SIG_IGN);
		if (pid == 0)
		{
			here_doc_manage(data, data->heredocs[i].fd);
			exit(1);
		}
		waitpid(pid, NULL, 0);
		signal(SIGINT, ft_builtin_ctrl_c);
		close(data->heredocs[i].fd[1]);
		i++;
	}
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
	// printf("%scommand->nb_pipes = %d %s\n", YELLOW, data.command->nb_pipes, RESET); // PRINT
	data.current->flag = 0;
	// while (data.current)
	// {
	// 	open_fd(data.current);
	// 	data.current = data.current->next;
	// }
	data.current = current;
	handle_execve_processes(&data, env);
	wait_for_children(data.command, data.child_pids);
	cleanup(data.child_pids, data.infile);
}
