/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/28 16:28:45 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

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
	t_process_data	data_main;
	t_command	*new_cmd;
	char		*input;
	int			flag_ok;

	flag_ok = 0;
	while (1)
	{
		input = readline("minishell$> ");
		add_history(input);
		ctrl_d_main(input, new_cmd, env_bis, flag_ok);
		// ft_builtin_write_exit(input);
		if (error_input(env_bis, new_cmd, input, flag_ok) == 2
			|| verif_nb_quote(input) != 0
			|| pipe_syntax_errors(input) == 2)
			continue ;
		new_cmd = get_command(input, env_bis);
		count_and_set_pipes(input, new_cmd);
		if (new_cmd != NULL)
		{
			new_cmd->command_arg_main = parse_input_quote(new_cmd->command);
			new_cmd->export_arg = parse_arg_export(new_cmd->command);
			execve_builtins_unset_export(new_cmd, env_bis);
			execve_builtin_cd(new_cmd, env_bis);
			// ft_free_tab(new_cmd->command_arg);
			data_main = execve_fd(new_cmd, env_bis);

			if (new_cmd->command_arg_main)
				ft_free_tab(new_cmd->command_arg_main);

			// free_child(&data_main, env_bis);
			
			// ft_close_all_fd();

			// ft_free_all(new_cmd, new_cmd->token_head); // 38/730 + SEG

			// if (flag_ok != 0)
			(void)data_main;
			if (new_cmd->token_head)
				ft_free_token(new_cmd);
			if (new_cmd)
				ft_free_current(new_cmd);
		}
		flag_ok = 1;
		free(input);
	}
	ft_free_env(env_bis);
	ft_free_all(new_cmd, new_cmd->token_head);
	clear_history();
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env_bis;

	// if (isatty(0))
	// {
		if (check_args_and_env(ac, envp))
			return (1);
		env_bis = initialize_env(envp);
		if (!env_bis)
			return (1);
		main_loop(env_bis);
	// }
	// else
	// {
	// 	printf("the standard input is NOT from a terminal\n");
	// 	return (-1);
	// }
	(void)av;
	return (0);
}

/*
                                        TO DO :

leaks a corriger :

sur les heredooc : 
cat << a | cat << b
leaks sur les hd et les builtins apres pipe.
=> here_doc_ray leaks sur les cmd apres pipe. exemple : cat << a | pwd

leaks sur les ctrl C des hd

sur les echo :
jump partout

<Makefile


/////////////////////////////////////////////////
sous tapis ? : 

echo				a
doit ignorer les tabs

export PATH=$PATH:$PWD
	-> "minishell" doit marcher comme "./minishell"
export :xcxcxc
	-> casse apres si clear au 1er, doit etre fait 2x pour fonctionner

 echo "$PATH" | '/usr/bin/wc'      -c    > 'coucou>tamere'
	-> cree 2 files, probleme avec la tokenisation

penser a rechercher les truc quon a (void) et voir si utile.
pareil pour forbiden function et a recoder

*/