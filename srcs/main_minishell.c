/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/07 11:59:42 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// // builtin export
// // variable globale pour exit

int	main(int ac, char **av, char **envp)
{
	t_command	*current;
	char		*input;
	int			input_fd;
	int			output_fd;

	while (1)
	{
		input = readline("minishell$> ");
		if (!input || strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		// input_fd = open(av[1], O_RDONLY);
		// output_fd = open(av[4], O_CREAT | O_RDWR | O_TRUNC, 0644); // av[4]
		current = get_command_and_separator(input);
		// if (current->command_arg[1])
		// {
			input_fd = open(current->command_arg[1], O_RDONLY);
			if (input_fd == -1)
			{
				perror("Error opening input file");
				exit(EXIT_FAILURE);
			}
			output_fd = open(current->command_arg[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
		// else
		// 	input_fd = 0;  // stdin
		
		multiple_pipe(current, envp, input_fd, output_fd);

		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}
