/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:39:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/11 17:39:30 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_builtin_exit(char *input)
{
	if (!input || ft_strcmp_minishell(input, "exit") == 0)
	{
		printf("exit\n");
		free(input);
		exit (0);
	}
}

void handle_sigint (int sig) 
{
	(void)sig;
	write(1, "\nminishell$> ", 13);
}