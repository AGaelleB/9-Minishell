/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/02 12:09:46 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* int	main(int ac, char **av, char **envp)
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
			if (new_commands != NULL)
			{
				env_bis->flag_builtin = false;
				new_commands->command_arg = parse_input_quote(new_commands->command);
				pid_t pid = fork();
				execve_builtins_unset_export(new_commands, env_bis);
				// execve_builtin_cd(new_commands, env_bis);
				// printf("Avant l'appel à execve_builtin_cd à la ligne %d\n", __LINE__);
				execve_builtin_cd(new_commands, env_bis);
				// printf("Après l'appel à execve_builtin_cd à la ligne %d\n", __LINE__);
				if (pid == 0)  // child process
				{
					execve_fd(new_commands, env_bis);
					exit(0);
				}
				else if (pid < 0)  // fork failed
					perror("fork");
				else  // parent process
					waitpid(pid, NULL, 0);
			}
			g_ctrl_c_pressed = 0;
			// clean_heredoc_files(new_commands);
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
} */

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
	t_command *new_commands;
	t_env *env_bis;
	char *input;

	(void)av;
	env_bis = (t_env *)malloc(sizeof(t_env));
	if (!env_bis)
		return (1);
	if (isatty(0))
	{
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
			if (error_input(input) == 2 || verif_nb_quote(input) != 0 || pipe_syntax_errors(input) == -1)
			{
				free(input);
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
					execve_fd(new_commands, env_bis);
					exit(0);
				}
				else if (pid < 0)
					perror("fork");
				else
					waitpid(pid, NULL, 0);
				g_ctrl_c_pressed = 0;
				ft_free_struct(new_commands, new_commands->token_head);
				ft_free_current(new_commands);
				free(input);
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
