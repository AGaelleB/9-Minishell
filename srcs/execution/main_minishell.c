/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/13 16:30:55 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int main(int ac, char **av, char **envp)
{
	char        *input;
	t_command   *new_commands;
	int         builtin_status;

	new_commands = NULL;
	signal(SIGINT, handle_sigint);
	while (1)
	{
		input = readline("minishell$> ");
		ft_builtin_ctrl_D(input);
		builtin_status = ft_all_builtins_exit(input);
		if (builtin_status == 1)
		{
			free(input);
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
	close(new_commands->fd[1]);
	close(new_commands->fd[0]);
	ft_free_all_cmd(new_commands);
	free(new_commands);
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}


/*
builtins a faire : 
- cd
- 




**********************

ETAPE 1 : 

-> pour resoudre faire une free de l'input apres le ctrl_C

 ./minishell
minishell$> hgfkg^C
minishell$> dghlhd^C
minishell$> exit
minishell: command not found: hgfkgdghlhdexit
minishell$>




ETAPE 2 : 

-> corriger les leaks pour les fausses commandes 
valgrind --leak-check=full --show-leak-kinds=all ./minishell



*/