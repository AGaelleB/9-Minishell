/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/10 14:42:17 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;


void	child_main(t_command *current, t_env *env)
{
	int		status;
	pid_t	pid;
	pid_t	*child_pids;
	int i;

	i = 0;
	init_execve(current, &(child_pids));
	while (current)
	{
		// if (current == NULL)
		// 	break ;
		pid = fork();
		child_pids[i++] = pid;
		if (pid == 0)
		{
			execve_fd(current, env);
			ft_close_all_fd();
			ft_free_all(current, current->token_head);
			exit(g_exit_status);
		}
		else if (pid < 0)
			perror("fork");
		else
		{
			signal(SIGINT, SIG_IGN);
			waitpid(pid, &status, 0);
			// i = 0;
			// while (i <= current->nb_pipes)
			// {
			// 	waitpid(child_pids[i], &status, 0);
			// 	i++;
			// }
			signal(SIGINT, ft_builtin_ctrl_c);
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
		}
		current = current->next;
	}
}

int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	t_env		*env_bis;
	char		*input;

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
			if (error_input(input) == 2 || verif_nb_quote(input) != 0 || pipe_syntax_errors(input) == 2)
				continue;
			add_history(input);
			new_commands = get_command(input, env_bis);
			count_and_set_pipes(input, new_commands);
			if (new_commands != NULL)
			{
				new_commands->command_arg = parse_input_quote(new_commands->command);
				new_commands->export_arg = parse_arg_export(new_commands->command); //not free
				execve_builtins_unset_export(new_commands, env_bis);
				execve_builtin_cd(new_commands, env_bis);
				child_main(new_commands, env_bis);
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
echo " 'O $USER O'     "

<<un <<deux cat | <<trois cat
=> n affiche pas le dernier heredoc, faire en sorte d ignorer si << apres pipe 

EOF et ctrl^c leaks

Tapis ?: OUI
minishell$> cat << " 'a' " << "b" << 'c'

penser a rechercher les truc quon a (void) et voir si utile.
pareil pour forbiden function et a recoder

*/
