/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:02:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/11 15:13:09 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_all_builtins(char *input)
{
	if (ft_builtin_enter(input) != 0)
		return (2);
	if (ft_builtin_write_exit(input) != 0)
	{
		free(input);
		exit(0);
	}
	if (ft_is_all_space(input) != 0)
		return (2);
	if (ft_strcmp_minishell(input, "\"\"") == 0
			|| ft_strcmp_minishell(input, "\'\'") == 0)
	{
		printf("minishell: : command not found\n");
		free(input);
		return (2);
	}
	return (0);
}

void	ft_all_builtins_verif(t_command *current)
{
	int	cmd_count;

	cmd_count = 0;
	while (current)
	{
		// printf("%sCommand %d: %s%s\n", GREEN, cmd_count, current->command, RESET);
		if (ft_strncmp(current->command, "pwd", 3) == 0)
		{
			ft_builtin_pwd_fd(STDOUT_FILENO);
			exit (0);
		}
		if (ft_strncmp(current->command, "echo", 4) == 0)
		{
			if (current->command_path != NULL)
			{
				ft_builtin_echo_fd(current->command_arg);
				exit (0);
			}
		}
		cmd_count++;
		current = current->next;
		// printf("\n");  // Just to separate different commands
	}
}
