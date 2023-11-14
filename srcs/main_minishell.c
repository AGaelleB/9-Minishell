/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/13 17:29:28 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;


void	child_main(t_command *current, t_env *env)
{
	int		status;
	pid_t	pid;

	pid = fork();
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
		signal(SIGINT, ctrl_c_main);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}

int	check_args_and_env(int ac, char **envp)
{
	if (ac != 1)
		return (printf("run ./minishell without arg\n"));
	if (!envp[0])
		return (printf("env is missing\n"));
	return (0);
}

t_env	*initialize_env(char **envp)
{
	t_env	*env_bis;

	g_exit_status = 0;
	signal(SIGINT, ctrl_c_main);
	signal(SIGQUIT, SIG_IGN);
	env_bis = (t_env *)malloc(sizeof(t_env));
	if (!env_bis)
		return (NULL);
	copy_env(env_bis, envp);
	return (env_bis);
}

void	main_loop(t_env *env_bis)
{
	t_command	*new_cmd;
	char		*input;

	while (1)
	{
		input = readline("minishell$> ");
		ft_builtin_ctrl_d(input);
		ft_builtin_write_exit(input);
		if (error_input(input) == 2 || verif_nb_quote(input) != 0
			|| pipe_syntax_errors(input) == 2)
			continue ;
		add_history(input);
		new_cmd = get_command(input, env_bis);
		count_and_set_pipes(input, new_cmd);
		if (new_cmd != NULL)
		{
			new_cmd->command_arg = parse_input_quote(new_cmd->command);
			new_cmd->export_arg = parse_arg_export(new_cmd->command); //not free
			execve_builtins_unset_export(new_cmd, env_bis);
			execve_builtin_cd(new_cmd, env_bis);
			child_main(new_cmd, env_bis);
		}
		free(input);
	}
	ft_close_all_fd();
	ft_free_env(env_bis);
	ft_free_all(new_cmd, new_cmd->token_head);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env_bis;

	if (isatty(0))
	{
		if (check_args_and_env(ac, envp))
			return (1);
		env_bis = initialize_env(envp);
		if (!env_bis)
			return (1);
		main_loop(env_bis);
	}
	else
	{
		printf("the standard input is NOT from a terminal\n");
		return (-1);
	}
	(void)av;
	return (0);
}

/*
										TO DO :

EOF et ctrl^c leaks

penser a rechercher les truc quon a (void) et voir si utile.
pareil pour forbiden function et a recoder

*/
