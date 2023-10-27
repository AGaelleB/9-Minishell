/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:04:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/27 17:41:53 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

char	*create_heredoc(void)
{
	char	*heredoc;
	int		i;

	i = 0;
	heredoc = malloc(sizeof(char) * 11);
	if (!heredoc)
		return (NULL);
	heredoc[10] = '\0';
	while (i < 10)
	{
		heredoc[i] = (char)aleatori_char();
		i++;
	}
	return (heredoc);
}

// static void	signal_cmd_2(int sig)
// {
// 	// g_exit_status += sig;
// 	if (sig == 2)
// 	{
// 		// g_exit_status = 130;
// 		printf("\n");
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// }

// static void	signal_cmd(int sig)
// {
// 	// g_exit_status += sig;
// 	if (sig == 2)
// 	{
// 		// g_exit_status = 130;
// 		printf("\n");
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 	}
// 	if (sig == SIGQUIT)
// 	{
// 		write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
// 		exit (1);
// 	}
// }

// void	sighandler_heredoc(int sig)
// {
// 	(void)sig;
// 	printf("\n");
// 	global_ctrl_c_pressed = 130;
// 	return ;
// }

void	sighandler_heredoc(int sig)
{
	int fd;

	(void)sig;
	fd = open ("/dev/null", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	printf("\n");
	// global_ctrl_c_pressed = 130; // 130 = ctrl C
	return ;
}

// void handle_signals_heredoc()
// {
// 	global_ctrl_c_pressed = 0;
// 	signal(SIGINT, sighandler_heredoc);
// 	signal(SIGQUIT, SIG_IGN);
// }

int	ctrl_d_heredoc(char *input, int i, char *delimiter)
{
	if (!input)
	{
		write(1, "minishell: warning: here-document at line ", 43);
		ft_putnbr_fd(i, STDOUT_FILENO);
		write(1," delimited by end-of-file (wanted \'", 36);
		ft_putstr_fd(delimiter, STDOUT_FILENO);
		write(1, "')\n", 4);
		return (45);
	}
	return(0);
}

// cat << eof | wc


int	write_in_fd(int fd, char *delimiter, t_command *current)
{
	char	*line;
	int		i;

	i = 0;
	while (1)
	{
		// handle_signals_heredoc();
		line = readline("> ");
		if (!line) // Si readline renvoie NULL, il se pourrait que CTRL+D ait été appuyé.
		{
			// Gérer EOF (CTRL+D) ici, si nécessaire.
			break;
		}
		(void)current;
		// if (global_ctrl_c_pressed) // Si CTRL+C est appuyé, nous nettoyons et sortons de la boucle.
		// {
		// 	printf("OMGGGGG\n");
		// 	free(line);
		// 	global_ctrl_c_pressed = 0; // réinitialiser pour de futures utilisations
		// 	return (-1); // retourner une erreur ou un code spécifique pour indiquer que CTRL+C a été pressé
		// }

		if (ft_strcmp_minishell(line, delimiter) == 0)
			break ;

		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		i++;
		// free(line);
	}
	// free(line); // Assurez-vous que c'est sûr en cas de 'line' est NULL.
	return (i > 0 ? 0 : -1); // Vérifiez si quelque chose a été écrit ou non.
}



// int	write_in_fd(int fd, char *delimiter, t_command *current)
// {
// 	// printf("write_in_fd \n");
// 	char	*line;
// 	int		i;

// 	i = 0;
// 	// handle_signals_heredoc();
// 	// signal(SIGINT, signal_cmd_2);
// 	// signal(SIGINT, signal_cmd);
// 	// signal(SIGQUIT, SIG_IGN);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (global_ctrl_c_pressed == 130)
// 		{
// 			// clean_heredoc_files(current);
// 			free(line);
// 			break;
// 			// exit(130);
// 			// return (130);
// 			(void)current;
// 		}
// 		if (ctrl_d_heredoc(line, i, delimiter) == 45)
// 			return (45);
// 		if (ft_strcmp_minishell(line, delimiter) == 0)
// 			break ;
// 		write(fd, line, ft_strlen(line));
// 		write(fd, "\n", 1);
// 		i++;
// 		// signal(SIGINT, sighandler_heredoc);
// 		// signal(SIGQUIT, SIG_IGN);
// 		// handle_signals_heredoc();
// 		free(line);
// 	}
// 	// signal(SIGINT, signal_cmd);
// 	// signal(SIGQUIT, SIG_IGN);
// 	// printf("END write_in_fd \n");
// 	free(line);
// 	return (0);
// }

void	add_to_heredocs_list(t_command *current, char *heredoc_name)
{
	int	i;
	
	i = 0;
	if (!current->heredocs)
	{
		current->heredocs = malloc(sizeof(char*) * 2);
		if (!current->heredocs)
			return ;
		current->heredocs[0] = ft_strdup(heredoc_name);
		current->heredocs[1] = NULL;
		return ;
	}
	while (current->heredocs[i])
		i++;
	current->heredocs = ft_realloc(current->heredocs, sizeof(char*) * (i + 1), sizeof(char*) * (i + 2));
	current->heredocs[i] = ft_strdup(heredoc_name);
	current->heredocs[i + 1] = NULL;
}

t_token	*handle_multiple_heredocs(t_command *current, t_token *token)
{
	// printf("handle_multiple_heredocs \n");
	char	*delimiter;
	int		fd;

	while (token && ft_strcmp_minishell(token->split_value, "<<") == 0)
	{
		delimiter = extract_filename_heredoc(current->command);
		// printf("delimiter = %s\n", delimiter);
		// if (current->heredoc)
		// 	free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
		add_to_heredocs_list(current, current->heredoc);
		write_in_fd(fd, delimiter, current);
		
		
		// int result = write_in_fd(fd, delimiter, current);
		// if (result == -1)
		// 	return (-1);



		
		fd = open(current->heredoc, O_RDONLY);
		current->fd_in = fd;
		if (current->fd_in == -1)
		{
			perror("minishell: EOF");
			exit(-1);
		}
		token = token->next->next;
	}
	return(token);
}
