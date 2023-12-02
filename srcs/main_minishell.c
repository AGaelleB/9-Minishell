/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/02 17:17:25 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

static void	process_command(t_command *new_cmd, t_env *env_bis)
{
	new_cmd->command_arg_main = parse_input_quote(new_cmd->command);
	new_cmd->export_arg = parse_arg_export(new_cmd->command);
	execve_builtins_unset_export(new_cmd, env_bis);
	execve_builtin_cd(new_cmd, env_bis);
	execve_fd(new_cmd, env_bis);
	if (new_cmd->command_arg_main)
		ft_free_tab(new_cmd->command_arg_main);
	if (new_cmd->token_head)
		ft_free_token(new_cmd);
}

static void	cleanup_command(t_command *new_cmd)
{
	if (new_cmd)
		ft_free_current(new_cmd);
}

static void	read_and_process_input(t_env *env_bis)
{
	t_command		*new_cmd;
	char			*input;
	int				flag_ok;

	flag_ok = 0;
	while (1)
	{
		input = readline("minishell$> ");
		add_history(input);
		ctrl_d_main(input, new_cmd, env_bis, flag_ok);
		if (error_input(env_bis, input) == 2
			|| verif_nb_quote(input) != 0
			|| pipe_syntax_errors(input) == 2)
			continue ;
		new_cmd = get_command(input, env_bis);
		count_and_set_pipes(input, new_cmd);
		if (new_cmd != NULL)
		{
			process_command(new_cmd, env_bis);
			cleanup_command(new_cmd);
		}
		flag_ok = 1;
		free(input);
	}
}

void	main_loop(t_env *env_bis)
{
	read_and_process_input(env_bis);
	ft_free_env(env_bis);
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
/////////////////////////////////////////////////
						BUGS

1/ 		pb sur le ctrl C d un hd qui affiche son contenu

/////////////////////////////////////////////////

penser a rechercher les truc quon a (void) et voir si utile.
pareil pour forbiden function et a recoder et warning



tapis: 
'e""ch'o 'd"f"'

export GAGA='$'USER
*/