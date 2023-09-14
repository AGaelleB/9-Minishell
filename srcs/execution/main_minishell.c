/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/14 12:13:58 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*new_commands = NULL;

// char	*return_input(char *input) // truc de rayan
// {
// 	static char *inp;

// 	if (input = NULL)
// 		return (inp);
// 	inp = input;
// }

void cleanup_resources(void)
{
	if (new_commands)
	{
		close(new_commands->fd[1]);
		close(new_commands->fd[0]);
		ft_free_all_cmd(new_commands);
		free(new_commands);
		new_commands = NULL;
	}
}

int main(int ac, char **av, char **envp)
{
	char		*input;
	int			builtin_status;
	
	signal(SIGINT, ft_signal_ctrl_C);
	while (1)
	{
		ctrl_c_pressed = 0; // Reset le flag en debut de boucle
		input = readline("minishell$> ");
		if (ctrl_c_pressed == 1)
		{
			free(input);
			continue;
		}
		ft_builtin_ctrl_D(input);
		builtin_status = ft_all_builtins_exit(input);
		if (builtin_status == 1)
		{
			free(input);
			cleanup_resources(); // NEW
			exit(0);
		}
		else if (builtin_status == 2)
			continue;
		new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			new_commands->token = tokenize_input(input);
			open_fd(new_commands, envp);
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
										A CORRIGER :

valgrind --leak-check=full --show-leak-kinds=all --memcheck:suppressions=./.minishell.supp ./minishell

-> corriger les leaks pour les fausses commandes 
-> l'exit du contr_D leak aussi si par exemple la commande d avant est valide
-> Invalid read si 1er commande est un enter
*/