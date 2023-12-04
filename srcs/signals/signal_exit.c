/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:29:45 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/04 16:41:16 by abonnefo         ###   ########.fr       */
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

t_process_data	*return_data(t_process_data *data)
{
	static t_process_data	*test_data;

	if (data)
		test_data = data;
	return (test_data);
}

t_env	*return_env(t_env *env)
{
	static t_env	*test_env;

	if (env)
		test_env = env;
	return (test_env);
}

void	ctrl_c_heredoc(int signal)
{
	t_process_data	*data;
	t_env			*env;

	data = return_data(NULL);
	env = return_env(NULL);
	if (signal == SIGINT)
	{
		write(2, "\n", 1);
		free(data->delimiter);
		ft_free_tab(data->command->command_arg_main);
		free_child(data, env);
		g_exit_status = 130;
		exit(g_exit_status);
	}
}
