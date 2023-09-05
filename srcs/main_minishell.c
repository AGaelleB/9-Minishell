/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/05 10:51:01 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	char *input;

	while (1)
	{
		input = readline("minishell$> ");

		if (!input)
		{
			// L'utilisateur a appuyé sur Ctrl+D (EOF) pour quitter.
			printf("exit\n");
			break;
		}

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
	(void)env;

	return 0;
}
