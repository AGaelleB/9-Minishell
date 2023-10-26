/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:41:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/26 09:53:50 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execve_builtins_unset_export(t_command *cmd, t_env *env_bis)
{
	if (ft_strncmp(cmd->command, "unset", 5) == 0)
	{
		if (cmd->nb_pipes != 0)
			env_bis->flag_builtin = false;
		else
			ft_builtin_unset(cmd->command_arg, env_bis);
	}
	else if (ft_strncmp(cmd->command, "export ", 7) == 0)
	{
		if (cmd->nb_pipes != 0)
			env_bis->flag_builtin = false;
		else
			ft_builtin_export(cmd->command_arg, env_bis);
	}
}

void	execve_builtin_cd(t_command *cmd, t_env *env_bis)
{
	if (ft_strcmp_minishell(cmd->command, "cd") == 0)
	{
		if (cmd->nb_pipes != 0)
			env_bis->flag_builtin = false;
		else
			ft_builtin_cd(cmd->command_arg, env_bis);
	}
	else if (ft_strncmp(cmd->command, "cd ", 3) == 0)
	{
		if (cmd->nb_pipes != 0)
			env_bis->flag_builtin = false;
		else
			ft_builtin_cd(cmd->command_arg, env_bis);
	}
}
