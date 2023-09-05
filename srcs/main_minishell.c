/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/05 15:28:14 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	char *input;

	while (1)
	{
		input = readline("minishell$> "); // utiliser GNL ? 

		if (!input)
		{
			// L'utilisateur a appuyé sur Ctrl+D (EOF) pour quitter.
			printf("exit\n");
			break;
		}
		ft_check_paths(envp, input);
		// Vous pouvez ajouter ici la logique de traitement de la commande
		// Par exemple, vous pouvez utiliser des instructions conditionnelles pour exécuter différentes actions en fonction de la commande entrée.

		if (strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		else
			printf("%s\n", input);
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	// (void)env;

	return 0;
}
