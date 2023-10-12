/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/12 14:26:26 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_execve(t_command *cur, pid_t **childs_pids)
{
	int	nb_child;

	nb_child = 0;
	while (cur)
	{
		(nb_child)++;
		cur = cur->next;
	}
	*childs_pids = malloc(nb_child * sizeof(pid_t));
	if (!*childs_pids)
		exit(1);
}

void	ft_set_args_and_paths(t_command *current, char **envp)
{
	current->command_arg = NULL;
	current->command_path = NULL;
	current->command_arg = parse_input_quote(current->command);

	int i = 0;
	while(current->command_arg[i])
	{
		printf("%s\ncommand_arg[%d] = %s%s", YELLOW, i, current->command_arg[i], RESET);
		i++;
	}
	printf("%s\n PATH command_arg[0] = %s%s", GREEN, current->command_arg[0], RESET);
	printf("\n");
	current->command_path = ft_check_paths(envp, current->command_arg[0]);
}

int	execve_process(t_command *current, char **envp)
{
	if (current->command_path == NULL)
	{
		write(2, "minishell: ", 11);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, " :command not found", 19);
		write(2, "\n", 1);
		ft_free_tab(current->command_arg);
		ft_free_current(current);
		return (127);
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1)
	{
		perror("Error");
		exit(-1);
	}
	return (0);
}
