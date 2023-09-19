/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/19 12:57:18 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char	*return_input(char *input) // truc de rayan
// {
// 	static char *inp;

// 	if (input = NULL)
// 		return (inp);
// 	inp = input;
// }

int main(int ac, char **av, char **envp)
{
	char		*input;
	int			builtin_status;
	t_command	*new_commands;
	
	new_commands = NULL;
	signal(SIGINT, ft_signal_ctrl_C);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell$> ");
		ft_builtin_ctrl_D(input);
		builtin_status = ft_all_builtins_exit(input);
		if (builtin_status == 1)
		{
			free(input);
			exit(0);
		}
		else if (builtin_status == 2)
			continue;

		new_commands = get_command(input);
		// print_commands_and_tokens(new_commands);
		
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			// new_commands->token = tokenize_input(input);
			// printf("%s\n %d\n\n", new_commands->token->split_value, new_commands->token->type);
			// if(new_commands->token != NULL)
				execve_fd(new_commands, envp);
		}
		add_history(input);
		ft_free_current(new_commands);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}

/*
										A CORRIGER :
mhoy
a faire : redirections 


seg fault:
minishell$> cat celine.txt | txt.txt
[1]    2038653 segmentation fault (core dumped)  ./minishell

*/