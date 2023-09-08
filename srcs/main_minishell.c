/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/08 17:00:26 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// // builtin export
// // variable globale pour exit

// int	main(int ac, char **av, char **envp)
// {
// 	t_command	*current;
// 	char		*input;

// 	current = NULL;
// 	while (1)
// 	{
// 		input = readline("minishell$> ");
// 		if (!input || strcmp(input, "exit") == 0)
// 		{
// 			printf("exit\n");
// 			free(input);
// 			break;
// 		}
// 		if(current != NULL)
// 		{
// 			current->token = tokenize_input(input);
// 			current = get_command(input);
// 			open_fd(current, current->token, envp);
// 		}
// 		add_history(input);
// 		free(input);
// 	}
// 	(void)ac;
// 	(void)av;
// 	(void)envp;
// 	return (0);
// }

int	main(int ac, char **av, char **envp)
{
	// t_command	*current;
	char		*input;

	// current = NULL;
// Déclaration initiale
// t_command *current = NULL;

	while (1) {
    input = readline("minishell$> ");
    if (!input || strcmp(input, "exit") == 0) {
        printf("exit\n");
        free(input);
        break;
    }
    
    t_command *new_commands = get_command(input);  // Nouvelle variable pour les commandes récemment parsées

    // Ici, `new_commands` contient la nouvelle chaîne de commandes,
    // et `current` reste inchangé pour que vous puissiez l'utiliser
    // pour d'autres opérations si nécessaire.
    
    if(new_commands != NULL) {
        new_commands->token = tokenize_input(input);
        open_fd(new_commands, new_commands->token, envp);
    }
    
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

1- choisir quel main utiliser 
2- debeuguer lles path pour voir si les chemins sont correctement initialises avant lexecve
3- probablement probleme avec les file descriptor 
4- implement er GNL pour la lecture du here doc ???
5-  jambon

*/