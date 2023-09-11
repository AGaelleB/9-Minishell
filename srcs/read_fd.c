/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/11 14:35:27 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void open_fd(t_command *current, t_token *token, char **envp)
{
	int fd[2];
	int infile = 0;
	pid_t pid;
	t_command *command;
	command = current;

	while (current)
	{
		if (pipe(fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		// Configurer l'entrée (stdin)
		current->read_fd = infile;
		if (current->next != NULL)
			current->write_fd = fd[1];
		else
			current->write_fd = 1; // stdout
		pid = fork();
		if (pid == 0)
		{
			// Enfant
			//close(fd[0]); // Fermer l'extrémité de lecture inutilisée dans l'enfant
			child_process(current, current->read_fd, current->write_fd, envp);
		}
		else if (pid > 0) 
		{
			// Parent
			// wait(NULL);  // Attendre la terminaison du processus enfant (optionnel)
			close(fd[1]); // Fermer l'extrémité d'écriture inutilisée dans le parent
			if (current->next != NULL)
				infile = fd[0]; // Pour la prochaine itération
			else
				close(fd[0]); // Fermer aussi l'extrémité de lecture si c'est la dernière commande
		} 
		else
		{
			perror("fork");
			exit(1);
		}
		current = current->next;
	}

	int status;
	int i = 0;
	// printf("nombre de pipes final : %d\n", command->nb_pipes);
	while (i < command->nb_pipes)
	{
		waitpid(0, &status, -1);
	}
	WEXITSTATUS(status);
	(void)token;
}