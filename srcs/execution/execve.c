/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/18 16:28:03 by bfresque         ###   ########.fr       */
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

void	ft_set_args_and_paths(t_command *cur, t_env *env)
{
	cur->command_arg = NULL;
	cur->command_path = NULL;
	cur->command_arg = parse_input_quote(cur->command);
/* 	int i = 0;
	while(cur->command_arg[i])
	{
		printf("%s\ncommand_arg[%d] = %s%s", YELLOW, i, cur->command_arg[i], RESET);
		i++;
	}
	printf("%s\n PATH command_arg[0] = %s%s", GREEN, cur->command_arg[0], RESET);
	printf("\n"); */
	cur->command_path = ft_check_paths(env, cur->command_arg[0]);
}

int	is_builtin(t_command *cur)
{
	if ((ft_strcmp_minishell(cur->command_arg[0], "cd") == 0)
		|| (ft_strncmp(cur->command_arg[0], "cd ", 3) == 0))
		return (1);
	if ((ft_strcmp_minishell(cur->command, "echo") == 0)
		|| (ft_strncmp(cur->command, "echo ", 5) == 0))
		return (1);
	if (ft_strcmp_minishell(cur->command, "env") == 0)
		return (1);
	if ((ft_strcmp_minishell(cur->command, "export") == 0)
		|| (ft_strncmp(cur->command, "export ", 7) == 0))
		return (1);
	if (ft_strcmp_minishell(cur->command, "pwd") == 0)
		return (1);
	if ((ft_strcmp_minishell(cur->command, "unset") == 0)
		|| (ft_strncmp(cur->command, "unset ", 6) == 0))
		return (1);
	return (0);
}

int	execve_process(t_command *cur, t_env *env)
{
	ft_set_args_and_paths(cur, env);
	if (env->flag_error)
		exit(0);
	if ((cur->command_path == NULL) && is_builtin(cur) == 0)
	{
		write(2, "minishell: ", 11);
		write(2, cur->command_arg[0], ft_strlen(cur->command_arg[0]));
		write(2, " :command not found", 19);
		write(2, "\n", 1);
		ft_free_tab(cur->command_arg);
		ft_free_current(cur);
		return (127);
	}
	else if ((cur->command_path)
		&& (execve(cur->command_path, cur->command_arg, env->cpy_env) == -1))
	{
		perror("Error");
		return (-1);
	}
	return (0);
}
