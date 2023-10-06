/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:29:45 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/06 15:29:47 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_builtin_ctrl_c(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_on_new_line(); // Informe readline que nous sommes sur une nouvelle ligne
	rl_replace_line("", 0); // Efface la ligne actuelle
	rl_redisplay(); // Réaffiche le prompt
}
