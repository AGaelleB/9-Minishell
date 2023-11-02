/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/02 17:14:26 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

/* int is_builtin_command(t_command *current)
{
	if (!current || !current->command)
		return (0);
	// En supposant que les fonctions check_* renvoient 1 si la commande correspond à un builtin
	if (check_pwd(current) || check_echo(current, NULL) || check_cd(current) ||
			check_env(current, NULL) || check_unset(current) || check_export(current, NULL))
	{ 
		return (1);
	}
	return (0);
} */

int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	t_env		*env_bis;
	char		*input;
	int			status;

	(void)av;
	env_bis = (t_env *)malloc(sizeof(t_env)); // A FREE
	// g_exit_status = 0;
	if (!env_bis)
		return (1);
	if (isatty(0))
	{
		if (ac != 1)
			return (printf("run ./minishell without arg\n"));
		if (!envp[0])
			return (printf("env is missing\n"));
		g_exit_status = 0;
		// printf("Debug: g_exit_status set to %d at main\n", g_exit_status);
		signal(SIGINT, ft_builtin_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		copy_env(env_bis, envp);
		while (1)
		{
			// printf("begin main %d\n", g_exit_status);
			input = readline("minishell$> ");
			// printf("after readline main %d\n", g_exit_status);
			ft_builtin_ctrl_d(input);
			if (error_input(input) == 2 || verif_nb_quote(input) != 0 || pipe_syntax_errors(input) == -1)
			{
				// free(input); // invalid free ???
				continue;
			}
			add_history(input);
			new_commands = get_command(input, env_bis);
			count_and_set_pipes(input, new_commands);
			if (new_commands != NULL)
			{
				new_commands->command_arg = parse_input_quote(new_commands->command);
				execve_builtins_unset_export(new_commands, env_bis);
				execve_builtin_cd(new_commands, env_bis);
				pid_t pid = fork();
				if (pid == 0)
				{
					// printf("if main %d\n", g_exit_status);
					execve_fd(new_commands, env_bis);
					// exit(64); // permet de kill les process des childs mais bug 
					// printf("if after execve main %d\n", g_exit_status);
					// return (64);
					exit(g_exit_status);
				}
				else if (pid < 0)
					perror("fork");
				else
				{
					// printf("else main %d\n", g_exit_status);
					waitpid(pid, &status, 0); // La valeur de status est mise à jour ici
					if (WIFEXITED(status))
						g_exit_status = WEXITSTATUS(status); // Mettez à jour g_exit_status avec le code de sortie du processus
				}
				// g_exit_status = 42;
				ft_free_struct(new_commands, new_commands->token_head);
				ft_free_current(new_commands);
				free(input);
				// printf("end main %d\n", g_exit_status);
			}
		}
	}
	else
	{
		printf("the standard input is NOT from a terminal\n");
		return (-1);
	}
	return (0);
}


/*
										TO DO :

EOF à faire :
	
	<<EOF ls
	> ^C
	ferme heredoc et ne fait rien 


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
