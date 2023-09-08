/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/08 16:19:25 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	open_fd(t_command *current, t_token *token, char **envp)
{
	int fd[2];
	int infile = 0;  // Par défaut, lire depuis stdin
	int outfile = 1; // Par défaut, écrire vers stdout

	while (current)
	{
		pipe(fd); // Créer un nouveau pipe pour cette commande

		// Configurer la sortie (stdout)
		if (current->next != NULL)
			current->output_fd = fd[1]; // La sortie sera l'entrée du pipe suivant
		else
			current->output_fd = outfile; // Sinon, utiliser le stdout

		// Configurer l'entrée (stdin)
		current->input_fd = infile;

		// Exécuter la commande
		exec_pipe(current, current->input_fd, current->output_fd, envp);

		// La sortie de cette commande sera l'entrée de la prochaine
		if (infile != 0)
			close(infile);
		close(current->output_fd);

		infile = fd[0]; // Jambon. Le read end du pipe devient l'entrée de la prochaine commande
		current = current->next;
		(void)token;
	}
}


