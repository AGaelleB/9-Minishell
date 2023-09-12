/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/12 13:46:52 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_command	*new_commands;

	new_commands = NULL;
	// block_signal(SIGQUIT);
	signal(SIGINT, handle_sigint);
	while (1)
	{
		input = readline("minishell$> ");
		if (!input) // Si input est NULL, CTRL-D a été pressé sur une ligne vide
		{
			write(1, "exit\n", 5);  // Nouvelle ligne pour quitter proprement
			exit(0);  // Quitter le shell
		}
		else if (ft_strcmp_minishell(input, "") == 0)  // Si input est une chaîne vide
		{
			free(input);
			continue;  // Retournez simplement au prompt
		}
		if (ft_all_builtins(input) != 0)
		{
			free(input);
			exit(0);
		}
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
	ft_free_all_cmd(new_commands);
	free(new_commands);
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}



/*
attention fin progam segfault si 1 cmd 


 ./minishell
minishell$> hgfkg^C
minishell$> dghlhd^C
minishell$> exit
minishell$> command not found :hgfkgdghlhdexit
command not found: hgfkgdghlhdexit





./minishell
minishell$> ls | cat test.txt 
coucou mots
c'est 
mémoire
mots
les mots

beaucoup de mots
minishell$> sdf
minishell$> command not found :sdf
command not found: sdf

[1]    542162 segmentation fault (core dumped)  ./minishell
*/