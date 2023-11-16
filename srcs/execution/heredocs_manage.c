/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs_manage.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 10:13:58 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/16 14:19:35 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	handle_input(char *input, int i, char *delimiter, int fd[2])
{
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
	return (0);
}

int	here_doc_manage(t_process_data *data, int fd[2], char *delimiter)
{
	char	*input;
	int		i;
	int		result;

	(void)data;
	i = 0;
	signal(SIGINT, ctrl_c_heredoc);
	close(fd[0]);
	while (1)
	{
		input = readline("> ");
		result = handle_input(input, i, delimiter, fd);
		if (result != 0)
			return (result);
		i++;
	}
	signal(SIGQUIT, ctrl_c_heredoc);
	return (0);
}

static int	count_heredocs(t_token *redir)
{
	int	count;

	count = 0;
	while (redir)
	{
		if (redir->type == TYPE_HEREDOC)
			count++;
		redir = redir->next;
	}
	return (count);
}

static int	manage_single_heredoc(t_process_data *data, int index)
{
	pid_t	pid;
	int		status;
	char	*delimiter;

	delimiter = epur_filename_heredoc(data->current->token_head);
	pipe(data->heredocs[index].fd);
	pid = fork();
	if (pid == 0)
	{
		here_doc_manage(data, data->heredocs[index].fd, delimiter);
		exit(1);
	}
	waitpid(pid, &status, 0);
	close(data->heredocs[index].fd[1]);
	return (WIFEXITED(status) && WEXITSTATUS(status) == 130);
}

int	here_doc_ray(t_process_data *data)
{
	int	i;
	int	heredoc_interrupted;

	data->count_hd = count_heredocs(data->current->token_head);
	// data->heredocs = malloc(sizeof(t_here_doc) * data->count_hd);
	data->heredocs = malloc(sizeof(t_here_doc));
	if (!data->heredocs)
		return (-1);
	signal(SIGINT, ctrl_c_heredoc);
	i = 0;
	heredoc_interrupted = 0;
	while (i < data->count_hd && !heredoc_interrupted)
	{
		heredoc_interrupted = manage_single_heredoc(data, i);
		i++;
	}
	signal(SIGQUIT, ctrl_c_heredoc);
	if (heredoc_interrupted)
		return (-1);
	else
		return (0);
}
