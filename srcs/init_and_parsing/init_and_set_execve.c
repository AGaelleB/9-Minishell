/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_set_execve.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 15:55:30 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/06 10:39:22 by abonnefo         ###   ########.fr       */
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

void	ft_set_args_and_paths(t_process_data *data, t_env *env)
{
	t_command	*cmd;

	cmd = data->command;
	while (cmd)
	{
		if (cmd->command_arg_main)
		{
			ft_free_tab(cmd->command_arg_main);
			cmd->command_arg_main = NULL;
		}
		cmd = cmd->next;
	}
	data->current->command_path = NULL;
	data->current->command = check_expr(data->current->command);
	data->current->command_arg = parse_input_quote(data->current->command);
	if (data->current->command_arg[0] != NULL)
		data->current->command_path = ft_check_paths(env,
				data->current->command_arg[0]);
	else
		data->current->command_path = NULL;
}

int	is_builtin(t_command *cur)
{
	if (ft_strcmp_minishell(cur->command, "exit") == 0)
		return (2);
	if ((cur->command_arg[0] != NULL))
	{
		if ((ft_strcmp_minishell(cur->command_arg[0], "cd") == 0)
			|| (ft_strncmp(cur->command_arg[0], "cd ", 3) == 0))
			return (1);
	}
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

int	check_args_and_env(int ac, char **envp)
{
	if (ac != 1)
		return (printf("run ./minishell without arg\n"));
	if (!envp[0])
		return (printf("env is missing\n"));
	return (0);
}

t_env	*initialize_env(char **envp)
{
	t_env	*env_bis;

	g_exit_status = 0;
	signal(SIGINT, ctrl_c_main);
	signal(SIGQUIT, SIG_IGN);
	env_bis = (t_env *)malloc(sizeof(t_env));
	if (!env_bis)
		return (NULL);
	return_env(env_bis);
	copy_env(env_bis, envp);
	return (env_bis);
}
