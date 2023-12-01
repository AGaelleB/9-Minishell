/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/01 15:57:58 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	handle_builtin_errors(t_process_data *data, t_env *env)
{
	free_child(data, env);
	exit(g_exit_status);
}

static void	handle_access_error(t_process_data *data, t_env *env)
{
	free_child(data, env);
	exit(126);
}

static void	handle_command_not_found(t_process_data *data, t_env *env)
{
	write(2, "minishell: ", 11);
	write(2, data->current->command_arg[0],
		ft_strlen(data->current->command_arg[0]));
	write(2, " :command not found", 19);
	write(2, "\n", 1);
	free_child(data, env);
	exit(g_exit_status = 127);
}

int	execve_process(t_process_data *data, t_env *env)
{
	ft_set_args_and_paths(data->current, env);
	if (env->flag_error || is_builtin(data->current) == 2)
		handle_builtin_errors(data, env);
	if (verif_access(data, env, data->current->command) == 1)
		handle_access_error(data, env);
	if (data->current->command_arg[0] == NULL)
		handle_builtin_errors(data, env);
	if ((data->current->command_path == NULL) && is_builtin(data->current) == 0)
		handle_command_not_found(data, env);
	else if ((data->current->command_path)
		&& (execve(data->current->command_path,
				data->current->command_arg, env->cpy_env) == -1))
		handle_command_not_found(data, env);
	return (0);
}
