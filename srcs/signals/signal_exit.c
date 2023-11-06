/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:29:45 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 15:15:11 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_status;

void	ft_builtin_ctrl_c(int signal)
{
	(void)signal;
	// write(1, "\n", 1);
	g_exit_status = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(1, "\n", 1);
	// rl_redisplay();
}

void	sighandler_heredoc(int sig)
{
	int	fd;

	(void)sig;
	fd = open ("/dev/null", O_RDONLY); // closed ? not sure
	dup2(fd, STDIN_FILENO);
	close(fd);
	printf("\n");
	g_exit_status = 130;
	return ;
}

void	handle_signals_heredoc(void)
{
	g_exit_status = 0;
	signal(SIGINT, sighandler_heredoc);
	signal(SIGQUIT, SIG_IGN);
}

int	ctrl_d_heredoc(char *input, int i, char *delimiter)
{
	if (!input)
	{
		write(1, "minishell: warning: here-document at line ", 43);
		ft_putnbr_fd(i, STDOUT_FILENO);
		write(1, " delimited by end-of-file (wanted \'", 36);
		ft_putstr_fd(delimiter, STDOUT_FILENO);
		write(1, "')\n", 4);
		return (45);
	}
	return (0);
}
