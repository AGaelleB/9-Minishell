/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/06 12:07:14 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	char *input;
	char *cmd_path;
	char **cmd_args;

	while (1)
	{
		input = readline("minishell$> ");

		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		//////////////////////
		cmd_args = ft_split(input, ' ');
		if (!cmd_args || !cmd_args[0])
			free(input);
		else
		{
			cmd_path = ft_check_paths(envp, cmd_args[0]);
			if (cmd_path)
			{
				if (execve(cmd_path, cmd_args, envp) == -1)
					perror("minishell"); // a gerer plus tard
				free(cmd_path);
			}
		}
		//////////////////////
		add_history(input);
		ft_free_tab(cmd_args);
		free(input);
	}
	(void)ac;
	(void)av;
	return (0);
}


// builtin export
// variable globale pour exit
// utiliser token
