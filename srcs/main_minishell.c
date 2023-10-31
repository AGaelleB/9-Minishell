/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/31 09:41:15 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void count_pipes(char *input, t_command *cmd) //doublons refaire appel a la fonction deja existante
// {
// 	int count = 0;
// 	while (*input)
// 	{
// 		if (*input == '|')
// 			count++;
// 		input++;
// 	}
// 	cmd->nb_pipes = count;
// }


int	main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	t_env		*env_bis;
	char		*input;

	(void)av;
	env_bis = (t_env *)malloc(sizeof(t_env));
	if (isatty(0))
	{
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
			ft_builtin_ctrl_d(input);
			if (error_input(input) == 2)
				continue;
			if (verif_nb_quote(input) != 0)
				continue;
			add_history(input);
			if (pipe_syntax_errors(input) == -1)
				continue;
			new_commands = get_command(input, env_bis);
			count_and_set_pipes(input, new_commands);
			// print_commands_and_tokens(new_commands); // PRINT
			if (new_commands != NULL)
			{
				// count_pipes(input, new_commands); // == count_and_set_pipes
				env_bis->flag_builtin = false;
				new_commands->command_arg = parse_input_quote(new_commands->command);
				execve_builtins_unset_export(new_commands, env_bis);
				execve_builtin_cd(new_commands, env_bis);
				execve_fd(new_commands, env_bis);
			}
			global_ctrl_c_pressed = 0;
			// ft_free_tab(new_commands->command_arg);
			ft_free_struct(new_commands, new_commands->token_head);
			ft_free_current(new_commands);
			free(input);
		}
	}
	else
	{
		printf("the standard input is NOT from a terminal\n");
		return(-1);
	}
	return (0);
}


/*
										TO DO :

EOF à faire : 
	
	cat << EOF | ls



minishell$> cat << EOF
> coucou
coucou
> EOF
malloc(): unaligned fastbin chunk detected 3



Dans votre code, lors de l'exécution d'une commande comme cat << EOF | wc, vous effectuez les étapes suivantes :

Vous créez un processus enfant pour gérer le heredoc (heredoc_open_fd_pipe).
Vous attendez que ce processus enfant termine avec waitpid(heredoc_pid, NULL, 0);.
Vous créez un autre processus enfant pour exécuter la commande cat (ou d'autres commandes si vous avez plusieurs pipes).
Vous attendez que ce dernier processus enfant termine avec waitpid(data.pid, NULL, 0);.
Le problème est que vous avez une situation où la sortie du heredoc (en tant que processus enfant)
 est censée être connectée à l'entrée de la commande cat. Cependant, si vous attendez que le heredoc termine avant de démarrer la commande cat,
  il n'y a rien pour lire la sortie du heredoc, et donc le pipe est bloqué.

Voici comment résoudre le problème :

Ne pas attendre que le processus heredoc termine avant de démarrer les autres processus enfant liés à la commande.
Après avoir démarré tous les processus enfants nécessaires pour la commande, attendez qu'ils terminent tous.















										A CORRIGER :
EXITSTATUS a faire; 

verifier les free valgrind etc
leaks lors de lexit apres avoir effectué une commande 
on doit boucle une premiere fois sur current pour avancé dans nos commandes pour ensuite free dans chaque commandes la tokenisation effectuée:
split_value avec "cat" et le token entier, cest ici que ce situe le probleme dinvalid read size






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


Pour les heredoc :
[42] $> cat << a << b << c
>coucou a
>a
>coucou b
>b
>coucou c
>c
coucou c
[42] $> unset PATH
[42] $> cat << a << b << c
>coucou a
>a
>coucou b
>b
>coucou c
>c
[42] $> 

=> devrait afficher minishell: cat: No such file or directory

************************************************************

*/
