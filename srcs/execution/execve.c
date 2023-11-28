/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/28 15:58:02 by abonnefo         ###   ########.fr       */
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
		return ;
}

void	ft_set_args_and_paths(t_command *cur, t_env *env)
{
	// int	i;

	// i = 0;
	// while (cur->command_arg[i])
	// 	cur->command_arg[i++] = NULL;
	
	if (cur->command_arg_main)
		ft_free_tab(cur->command_arg_main); // repare les leaks d une cmd not found mais casse des pipes
	// cur->command_arg = NULL;
	cur->command_path = NULL;
	cur->command_arg = parse_input_quote(cur->command);
	cur->command_path = ft_check_paths(env, cur->command_arg[0]);
}

int	is_builtin(t_command *cur)
{
	if (ft_strcmp_minishell(cur->command, "exit") == 0)
		return (2);
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

int	execve_process(t_process_data *data, t_env *env)
{
	ft_set_args_and_paths(data->current, env); // ici on free command_arg_main
	if (env->flag_error || is_builtin(data->current) == 2)
	{
		// ft_free_tab(data->command->command_arg_main); // test, if ? 
		free_child(data, env);
		usleep(1000);
		usleep(1000);
		exit(g_exit_status);
	}
	if (verif_access(data, env, data->current->command) == 1)
	{
		// ft_free_tab(data->command->command_arg_main); // test, if ? 
		free_child(data, env);
		exit(126);
	}
	usleep(1000);
	usleep(1000);
	if ((data->current->command_path == NULL) && is_builtin(data->current) == 0)
	{
		write(2, "minishell: ", 11);
		write(2, data->current->command_arg[0], ft_strlen(data->current->command_arg[0]));
		write(2, " :command not found", 19);
		write(2, "\n", 1);
		// ft_free_tab(data->command->command_arg_main); // test, if ? 
		free_child(data, env);
		exit(g_exit_status = 127);
	}
	else if ((data->current->command_path)
		&& (execve(data->current->command_path,
			data->current->command_arg, env->cpy_env) == -1))
	{
		write(2, "minishell: ", 11);
		write(2, data->current->command_arg[0], ft_strlen(data->current->command_arg[0]));
		write(2, " :command not found", 19);
		write(2, "\n", 1);
		// ft_free_tab(data->command->command_arg_main); // test, if ? 
		free_child(data, env);
		exit(g_exit_status = 127);
	}
	return (0);
}
