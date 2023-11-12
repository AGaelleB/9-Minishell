/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/12 12:12:07 by bfresque         ###   ########.fr       */
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
	signal(SIGINT, ctrl_c_main);
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

// void	handle_heredoc_signals(t_process_data *data, char *line)
// {
// 	if (g_exit_status == 130)
// 	{
// 		int i = 0;
// 		printf("handle_heredoc_signals\n");
// 		while (i < data->count_hd)
// 		{
// 			if (i == 0)
// 				break ;
// 			close(data->heredocs[i].fd[0]);
// 			close(data->heredocs[i].fd[1]);
// 			i++;
// 		}
// 		// free(line); // a free
// 		(void)line;
// 		g_exit_status = 130;
// 		exit(130);
// 	}
// }

// void	ctrl_c_heredoc(int signal)
// {
// 	t_process_data *data;
// 	int i;

// 	if (signal == SIGINT)
// 	{
// 		i = 0;
// 		while (i < data->count_hd)
// 		{
// 			if (i == 0)
// 				break ;
// 			close(data->heredocs[i].fd[0]);
// 			close(data->heredocs[i].fd[1]);
// 			i++;
// 		}
// 		//penser a free
// 		exit (42);
// 	}
// }

// void	ctrl_c_heredoc(int signal)
// {
// 	int	fd;

// 	if (signal == SIGINT)
//  	{
// 		fd = open ("/dev/null", O_RDONLY);
// 		dup2(fd, STDIN_FILENO);
// 		close(fd);
// 		printf("\n");
// 		g_exit_status = 130;
// 		exit(130);
// 	}
// }

void ctrl_c_heredoc(int signal)
{
    if (signal == SIGINT)
	{
        g_exit_status = 130;
		// close(fd[1]);
        printf("\n");
        exit(g_exit_status); 
    }
}


int	here_doc_manage(t_process_data *data, int fd[2], char *delimiter)
{
	char	*input;
	int i;
	// printf("%sdelimiter : %s\n%s", BLUE, delimiter, RESET);
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
			// printf("j'exit by ctrl^D\n");
			return (45);
		}
		if (ft_strcmp_minishell(input, delimiter) == 0)
		{
			free(input);
			// printf("j'exit by EOF\n");
			close(fd[1]);
			exit (1);
		}
		write(fd[1], input, ft_strlen(input));
		write(fd[1], "\n", 1);
		free(input);
		i++;
	}
		signal(SIGQUIT, ctrl_c_main); //sig
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
	signal(SIGINT, SIG_IGN); //sig

    // signal(SIGINT, ctrl_c_heredoc); // Configurez le gestionnaire de signal
    g_exit_status = 0;

	while (i < data->count_hd)
	{
		int status;

		waitpid(pid, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			// Si un heredoc a été interrompu, arrêtez d'attendre les autres
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

	// signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, ctrl_c_main); //sig
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
