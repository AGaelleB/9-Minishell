/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/22 14:18:30 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int	redirect_file_in(t_command *current, t_token *token) // <
{
	char	*filename;

	if (current->fd_in != 0)
		close(current->fd_in);
	filename = token->next->split_value;
	current->fd_in = open(filename, O_RDONLY);
	if (current->fd_in == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_file_out(t_command *current, t_token *token) // >
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

int	redirect_append_file_out(t_command *current, t_token *token) // >>
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

// void	fill_heredoc(char **original, char *new_str)
// {
// 	char	*temp;
	
// 	if (new_str == NULL)
// 		return;
// 	if (*original == NULL)
// 		*original = ft_strdup(new_str);
// 	else
// 	{
// 		temp = *original;
// 		*original = malloc(ft_strlen(temp) + ft_strlen(new_str) + 1);
// 		if (*original == NULL)
// 			return;
// 		strcpy(*original, temp); // same as strjoin //forbiden
// 		strcat(*original, new_str); // same as strjoin //forbiden
// 		free(temp);
// 	}
// }

int	aleatori_char(void)
{
	char	buff[4];
	int		nbr;
	int		fd;

	fd = open("/dev/random", O_RDONLY);
	if (fd < -1)
		return (-1);
	read(fd, buff, 4);
	nbr = *(int *)buff;
	if (nbr < 0)
		nbr++;
	if (nbr < 0)
		nbr = nbr * (-1);
	return ('a' + nbr % 26);
}

char	*create_file_name(void)
{
	char	*file_name;
	int		i;

	i = 0;
	file_name = malloc(sizeof(char) * 11);
	file_name[10] = '\0';
	while (i < 10)
	{
		file_name[i] = (char)aleatori_char();
		i++;
	}
	return (file_name);
}

int	write_in_fd(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
			return (45);
		if (ft_strcmp_minishell(line, delimiter) == 0)
			break;
		// if (str[0] != '\0') // A FAIRE
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	return (0);
}

int	redirect_heredoc(t_command *current, t_token *token) // >>
{
	char *delimiter;
	char *file_name;
	int fd;

	file_name = NULL;
	fd = -1;
	delimiter = token->next->split_value;
	if (fd == -1)
	{
		if(file_name)
			free(file_name);
		file_name = create_file_name();
		fd = open(file_name, O_CREAT | O_EXCL | O_RDWR, 0644);
	}
	write_in_fd(fd, delimiter);
	fd = open(file_name, O_RDONLY);
	current->fd_in = fd;
	free_file_name(current->file_name);
	if (current->fd_in == -1)
	{
		write(1, "minishell: ", 12);
		perror("EOF");
		exit(-1);
	}
	current->file_name = file_name;
	return (0);
}

int	open_fd(t_command *command)
{
	t_token	*token;

	token = command->token_head;
	while (token)
	{
		if (token->type == TYPE_HEREDOC)
		{
			if (redirect_heredoc(command, token) == 0)
			{
				dup2(command->fd_in, 0);
				close(command->fd_in);
			}
		}
		if (token->type == TYPE_REDIR_IN)
		{
			if (redirect_file_in(command, token) == 0)
			{
				dup2(command->fd_in, 0);
				close(command->fd_in);
			}
		}
		if (token->type == TYPE_REDIR_OUT)
		{
			if (redirect_file_out(command, token) == 0)
			{
				dup2(command->fd_out, 1);
				close(command->fd_out);
			}
		}
		if (token->type == TYPE_REDIR_APPEND)
		{
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
	// ft_free_tokens(command->token_head);
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