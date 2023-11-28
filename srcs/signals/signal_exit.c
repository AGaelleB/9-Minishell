/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:29:45 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/28 16:56:03 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ctrl_c_main(int signal)
{
	(void)signal;
	g_exit_status = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_redisplay();
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

t_process_data	*return_data(t_process_data *data) ////////////////////////
{
	static t_process_data	*test_data;

	if (data)
		test_data = data;
	return (test_data);
}

t_env	*return_env(t_env *env) ////////////////////////
{
	static t_env	*test_env;

	if (env)
		test_env = env;
	return (test_env);
}

// par contre je ne close surement pas bien mes 
// fd : close(data->heredocs[i].fd[0]);
void	ctrl_c_heredoc(int signal)
{
	// t_process_data *data;
	// t_env			*env;
	
	// data = return_data(NULL);
	// env = return_env(NULL);
	if (signal == SIGINT)
	{
		g_exit_status = 130;
		write(2, "\n", 1); // si coms n affiche plus C^ sur le readline
		//free a faire pour le ctrl^c dans les heredocs bcp de leaks

		// free_child(data, env);
		// printf("%sSIGNAL in ctrl C%s\n", CYAN, RESET);
		exit(g_exit_status);
	}
}
