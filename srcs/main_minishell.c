/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/07 17:45:37 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// // builtin export
// // variable globale pour exit

int	main(int ac, char **av, char **envp)
{
	// t_token		*token;
	t_command	*current;
	char		*input;

	while (1)
	{
		input = readline("minishell$> ");
		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		current->token = tokenize_input(input);
		current = get_command(input);
		open_fd(current, envp);
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}


/*
TO DO : 

1- denouer notre sac de noeux pour retirnir uniquement les elements necessaire> Doublns entre slit_value et command_arg
2- une fois trier, parametrer les fd_in ou fd_out
3- garder le groupe avec la commande et l'argument mais supprimer command agrs et garder 
le split pour avoir les elements du groupe individuelement et associer a leur type respectif

*/