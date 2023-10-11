/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/11 12:11:11 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if (ac != 1)
		return (printf("run ./minishell without arg\n"));
	if (!envp[0])
		return (printf("env is missing\n"));
	signal(SIGINT, ft_builtin_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell$> ");
		// input = readline("😈🔥 MINIHELL$> ");
		ft_builtin_ctrl_d(input);
		builtin_status = ft_all_builtins(input);
		if (builtin_status == 1)
		{
			free(input);
			exit(0);
		}
		else if (builtin_status == 2)
			continue;
		if (verif_nb_quote(input) != 0)
			continue;
		add_history(input);
		new_commands = get_command(input, envp);
		if(new_commands == NULL)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token \'", 2);
			ft_putchar_fd(input[0], 2);
			ft_putstr_fd("\'\n", 2);
			free (input);
			continue;
		}
		count_and_set_pipes(input, new_commands);
		// ft_all_builtins_verif(new_commands);
		// print_commands_and_tokens(new_commands); // PRINT
		if(new_commands != NULL)
			execve_fd(new_commands, envp);

		// ft_free_tab(new_commands->command_arg);
		ft_free_struct(new_commands, new_commands->token_head);
		ft_free_current(new_commands);
		free(input);
	}
	(void)av;
	return (0);
}


/*

										TO DO :

ajouté le texte aprés le file de redirection dans le file 
exemple: echo joli, > test beau 
cat test: joli,beau

gerer le fait qui'il puisse y avoir plusieurs fichier comportant des quote avant/apres la commande 
exemple: > "a" > "hello" > 'b' > c cat celine.txt
modifier la ligne token->current pour avamcer apres y etre passse



										
supprimer les files crees avec EOF lors de la gestion de l'env
faire d autres test sur les EOF
commencer les leaks 



										A CORRIGER :
verifier les free valgrind etc
leaks lors de lexit apres avoir effectué une commande 
on doit boucle une premiere fois sur current pour avancé dans nos commandes pour ensuite free dans chaque commandes la tokenisation effectuée:
split_value avec "cat" et le token entier, cest ici que ce situe le probleme dinvalid read size

voir si besoin de ferme mieux :	// free_file_name(current->file_name);


************************************************************
cassé chez Rayan : 
ec'h'o     t'rst'
	-> on doit avoir "trst"
ec'h'o'   ' t'rst'
	-> on doit avoir "echo   : command not found"

echo -n -a -nnn -er -nnnnnn -nae -nnn  bonjour
	-> -a -nnn -er -nnnnnn -nae -nnn  bonjourminishell$> 
************************************************************

*/