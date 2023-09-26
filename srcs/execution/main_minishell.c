/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/26 15:17:59 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	*return_input(char *input) // truc de rayan
// {
// 	static char *inp;

// 	if (input = NULL)
// 		return (inp);
// 	inp = input;
// }

int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	char		*input;
	int			builtin_status;
	
	new_commands = NULL;
	signal(SIGINT, ft_signal_ctrl_C);
	signal(SIGQUIT, SIG_IGN);
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

		if (verif_nb_single_quote(input) != 0)
		{
			printf("minishell: miss quote\n");
			free(input);
			continue;
		}

		new_commands = get_command(input);
		// print_commands_and_tokens(new_commands); // PRINT
		
		count_and_set_pipes(input, new_commands);
		
////////////////////////////////////////////////////////////////////////
		t_quote *substr_list = parse_input(input);
		ft_cat_list_quote(substr_list);
/////////////////////////////////////////////////////////////////////////
		
		if(new_commands != NULL)
			execve_fd(new_commands, envp);
		add_history(input);
		ft_free_struct(new_commands, new_commands->token_head);
		ft_free_current(new_commands);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}


/*
										A CORRIGER :

leaks lors de lexit apres avoir effectué une commande 
on doit boucle une premiere fois sur current pour avancé dans nos commandes pour ensuite free dans chaque commandes la tokenisation effectuée:
split_value avec "cat" et le token entier, cest ici que ce situe le probleme dinvalid read size

avec cette cmd on a de temps en temps le message d erreur "srcs: Is a directory" mais pas a tous les coups
cat celine.txt | rev  > tesssssssst.txt > a > b > srcs > d > e > f > g > h > fiiiin | ls  > lsss

devrait effectuer la redirection mais ca ne le fait pas car on split sur les espaces
minishell$> ls> coucou
minishell$> ls >coucou
minishell$> ls > coui>coucou
pour fix -> modifier la facon de tokenizer en parcouarnt word[i] et en regardant si ca comprend un ">"

voir si besoin de ferme mieux :	// free_file_name(current->file_name);


SINGLE QUOTE : 

bash-5.1$ 'ech'o' c'oucou
bash: echo coucou: command not found

bash-5.1$ 'ech'o     '  '  coucou
   coucou
   
bash-5.1$ 'ech'o     '    c'oucou
    coucou



*/