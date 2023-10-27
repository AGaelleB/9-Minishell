/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:29:45 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/27 17:32:34 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int global_ctrl_c_pressed = 0; // variable globale init ici

// void	ft_builtin_ctrl_c(int signal)
// {
// 	(void)signal;
// 	write(1, "\n", 1);
// 	global_ctrl_c_pressed = 1;
// 	rl_on_new_line(); // Informe readline que nous sommes sur une nouvelle ligne
// 	rl_replace_line("", 0); // Efface la ligne actuelle
// 	rl_redisplay(); // Réaffiche le prompt
// }

void	ft_builtin_ctrl_c(int sig)
{
	// g_exit_status += sig;
	if (sig == 2)
	{
		// g_exit_status = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (sig == SIGQUIT)
	{
		write(2, "Quit (core dumped)\n", ft_strlen("Quit (core dumped)\n"));
		exit (1);
	}
}