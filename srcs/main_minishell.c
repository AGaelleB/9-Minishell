/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/07 14:24:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	t_env		*env_bis;
	char		*input;
	int			status;

	(void)av;
	if (isatty(0))
	{
		if (ac != 1)
			return (printf("run ./minishell without arg\n"));
		if (!envp[0])
			return (printf("env is missing\n"));
		g_exit_status = 0;
		signal(SIGINT, ft_builtin_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		env_bis = (t_env *)malloc(sizeof(t_env));
		if (!env_bis)
			return (1);
		copy_env(env_bis, envp);
		while (1)
		{
			input = readline("minishell$> ");
			ft_builtin_ctrl_d(input);
			ft_builtin_write_exit(input);
			if (error_input(input) == 2 || verif_nb_quote(input) != 0 || pipe_syntax_errors(input) == -1)
				continue;
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
					ft_close_all_fd();
					ft_free_all(new_commands, new_commands->token_head);
					exit(g_exit_status);
				}
				else if (pid < 0)
					perror("fork");
				else
				{
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))
						g_exit_status = WEXITSTATUS(status);
				}
			}
			free(input);
		}
		ft_close_all_fd();
		ft_free_env(env_bis);
		ft_free_all(new_commands, new_commands->token_head);
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

<<un <<deux cat | <<trois cat
=> n affiche pas le dernier heredoc, faire en sorte d ignorer si << apres pipe 

[42] $> export HELLO-=123
minishell: export: `HELLO-': not a valid identifier

minishell$> export GAGA="'gaga'"
export GAGA="'gaga'"

minishell$> export = 
minishell: export: `=': not a valid identifier

EOF et ctrl^c leaks

 aie aie aie jai peur :
	minishell$> cat << " 'a' " << "b" << 'c'
	delimiter =  'a'  | current->heredoc = xguhpidyod 
	>  'a' 
	delimiter = b | current->heredoc = hyuuglvvda 
	> minishell: warning: here-document at line 0 delimited by end-of-file (wanted 'b')
	delimiter = c | current->heredoc = vbtsjdkezs 
	> minishell: warning: here-document at line 0 delimited by end-of-file (wanted 'c')

penser qa rechercher les truc quon a (void) et voir si utile. pareil pour forbiden function et a recoder

*/
