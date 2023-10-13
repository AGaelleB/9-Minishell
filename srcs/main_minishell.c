/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/13 18:05:02 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	t_env		*env_bis;
	char		*input;

	env_bis = (t_env *)malloc(sizeof(t_env));
	if (!env_bis)
		return (0);
	if (ac != 1)
		return (printf("run ./minishell without arg\n"));
	if (!envp[0])
		return (printf("env is missing\n"));
	signal(SIGINT, ft_builtin_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	copy_env(env_bis, envp);
	while (1)
	{
		input = readline("minishell$> ");
		// input = readline("😈🔥 MINIHELL$> ");
		ft_builtin_ctrl_d(input);
		if (ft_all_builtins(input) == 2)
			continue;
		if (verif_nb_quote(input) != 0)
			continue;
		add_history(input);
		if (pipe_syntax_errors(input) == -1)
			continue;
		new_commands = get_command(input, env_bis);
		count_and_set_pipes(input, new_commands);
		// print_commands_and_tokens(new_commands); // PRINT
		if(new_commands != NULL)
			execve_fd(new_commands, env_bis);
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

EOF a faire

ajouté le texte aprés le file de redirection dans le file 
exemple: echo joli, > test "    beau"
cat test: joli, beau

minishell$> echo test > "out file" test1
value : echo | type: 0
value : test | type: 1
value : > | type: 4
value : "out | type: 8
value : file" | type: 1
value : test1 | type: 1

res : test file" test1

minishell$> echo "test" > out "beau"
value : echo | type: 0
value : "test" | type: 1
value : > | type: 4
value : out | type: 8
value : "beau" | type: 1

res : "test" "beau"

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

ls |
	-> il execute la commande 
************************************************************

*/

/* char	*return_input(char *input) // truc de rayan
{
	static char *inp;

	if (input = NULL)
		return (inp);
	inp = input;
} */