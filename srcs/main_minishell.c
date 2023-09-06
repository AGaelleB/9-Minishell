/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/06 17:14:35 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// // builtin export
// // variable globale pour exit

int	main(int ac, char **av, char **envp)
{
	char *input;
	// char *cmd_path;
	// char **cmd_args;
	// char **prem_argc;
	while (1)
	{
		input = readline("minishell$> ");
		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		set_command_list(input);
		// int i = 0;
		// while(input[i])
		// {
		// 	if(input[i] == '|')
		// 	{
		// 		prem_argc = split_string(input, '|');
		// 		cmd_args = split_string(prem_argc[0], ' ');
		// 		if (!cmd_args || !cmd_args[0])
		// 			free(input);
		// 		else
		// 		{
		// 			cmd_path = ft_check_paths(envp, cmd_args[0]);
		// 			if (cmd_path)
		// 			{
		// 				if (execve(cmd_path, cmd_args, envp) == -1)
		// 					perror("minishell"); // a gerer plus tard
		// 				free(cmd_path);
		// 			}
		// 		}
		// 	}
		// 	i++;
		// }
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}
