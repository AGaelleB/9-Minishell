/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:29:45 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/13 16:27:36 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

volatile int	g_unblock_sigquit = 0;

// void	block_signal(int signal)
// {
// 	t_sigset	sigset;

// 	sigemptyset(&sigset);
// 	sigaddset(&sigset, signal);
// //	Ajoute les signaux de l'ensemble aux
// //	signaux bloqués pour ce processus
// 	sigprocmask(SIG_BLOCK, &sigset, NULL);
// 	if (signal == SIGQUIT)
// 		printf("\e[36mSIGQUIT (ctrl-D) blocked.\e[0m\n");
// }

// // Fonction pour débloquer le signal spécifié
// void	unblock_signal(int signal)
// {
// 	t_sigset	sigset;

// 	sigemptyset(&sigset);
// 	sigaddset(&sigset, signal);
// //	Retire les signaux de l'ensemble des
// //	signaux bloqués pour ce processus
// 	sigprocmask(SIG_UNBLOCK, &sigset, NULL);
// 	if (signal == SIGQUIT)
// 		printf("\e[36mSIGQUIT (ctrl-\\) unblocked.\e[0m\n");
// }

void handle_sigint (int signal) 
{
	(void)signal;
	// if (signal != SIGINT)
	// 	return 
	write(1, "\nminishell$> ", 13);
	// block_signal(SIGINT);
	g_unblock_sigquit = 1;
	// unblock_signal(SIGINT);
}





// void set_signal_action(void)
// {
// //	Déclaration de la structure sigaction
// 	struct sigaction	act;

// //	Initialiser la structure à 0.
// 	ft_bzero(&act, sizeof(act));
// //	Nouvelle routine de gestion de signal
// 	act.sa_handler = &handle_sigint;
// //	Applique la nouvelle routine à SIGINT (ctrl-c)
// 	sigaction(SIGINT, &act, NULL);
// }

// int	main(void)
// {
// //	Change l'action par défaut de SIGINT (ctrl-c)
// 	set_signal_action();
// //	Bloque le signal SIGQUIT (ctrl-\)
// 	block_signal(SIGQUIT);
// //	Boucle infinie pour avoir le temps de faire ctrl-\ et
// //	ctrl-c autant de fois que ça nous chante.
// 	while(1)
// 	{
// //		Bloque le signal SIGINT le temps de lire la variable
// //		globale.
// 		block_signal(SIGINT);
// //		Si la routine de gestion de SIGINT a indiqué qu'elle a
// //		été invoquée dans la variable globale
// 		if (g_unblock_sigquit == 1)
// 		{
// //			SIGINT (ctrl-c) a été reçu.
// 			printf("\n\e[36mSIGINT detected. Unblocking SIGQUIT\e[0m\n");
// //			Débloque SIGINT et SIGQUIT
// 			unblock_signal(SIGINT);
// 			unblock_signal(SIGQUIT);
// 		}
// //		Sinon, on débloque SIGINT et on continue la boucle
// 		else
// 			unblock_signal(SIGINT);
// 		sleep(1);
// 	}
// 	return (0);
// }

