/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:29:45 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/31 10:38:01 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int global_ctrl_c_pressed = 0; // variable globale init ici

void	ft_builtin_ctrl_c(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	global_ctrl_c_pressed = 1;
	rl_on_new_line(); // Informe readline que nous sommes sur une nouvelle ligne
	rl_replace_line("", 0); // Efface la ligne actuelle
	rl_redisplay(); // Réaffiche le prompt
}

void	sighandler_heredoc(int sig)
{
	int fd;

	(void)sig;
	fd = open ("/dev/null", O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	printf("\n");
	global_ctrl_c_pressed = 130;
	return ;
}

void handle_signals_heredoc()
{
	global_ctrl_c_pressed = 0;
	signal(SIGINT, sighandler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

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
