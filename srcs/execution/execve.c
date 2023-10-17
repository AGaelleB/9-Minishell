/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/17 15:13:02 by bfresque         ###   ########.fr       */
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

void	ft_set_args_and_paths(t_command *current, t_env *env)
{
	current->command_arg = NULL;
	current->command_path = NULL;
	current->command_arg = parse_input_quote(current->command);

	// int i = 0;
	// while(current->command_arg[i])
	// {
	// 	printf("%s\ncommand_arg[%d] = %s%s", YELLOW, i, current->command_arg[i], RESET);
	// 	i++;
	// }
	// printf("%s\n PATH command_arg[0] = %s%s", GREEN, current->command_arg[0], RESET);
	// printf("\n");
	current->command_path = ft_check_paths(env, current->command_arg[0]);
}

int	is_builtin(t_command *current)
{
	if (ft_strcmp_minishell(current->command_arg[0], "cd") == 0)
		return (1);
	if (ft_strcmp_minishell(current->command_arg[0], "echo") == 0)
		return (1);
	if (ft_strcmp_minishell(current->command_arg[0], "env") == 0)
		return (1);
	if (ft_strcmp_minishell(current->command_arg[0], "export") == 0)
		return (1);
	if (ft_strcmp_minishell(current->command_arg[0], "pwd") == 0)
		return (1);
	if (ft_strcmp_minishell(current->command_arg[0], "unset") == 0)
		return (1);
	return (0);
}


int	execve_process(t_command *current, t_env *env)
{
	ft_set_args_and_paths(current, env);
	if (env->flag_error)
		exit(0);
	if ((current->command_path == NULL) && (!env->flag_error)
		&& is_builtin(current) == 0)
	{
		write(2, "minishell: ", 11);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, " :command not found", 19);
		write(2, "\n", 1);
		ft_free_tab(current->command_arg);
		ft_free_current(current);
		return (127);
	}
	if(env->flag_builtin == true)
		exit(-1);
	else if ((current->command_path) &&
		(execve(current->command_path, current->command_arg, env->cpy_env) == -1))
	{
		perror("Error");
		return (-1);
	}
	return (0);
}
