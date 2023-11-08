/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 15:41:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 10:07:01 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	execve_builtins_unset_export(t_command *cmd, t_env *env_bis)
{
	if (ft_strncmp(cmd->command, "unset", 5) == 0 && cmd->nb_pipes == 0)
			ft_builtin_unset(cmd->command_arg, env_bis);
	else if ((ft_strncmp(cmd->command, "export ", 7) == 0
		|| ft_strcmp_minishell(cmd->command, "export") == 0)
		&& (cmd->nb_pipes == 0))
			ft_builtin_export(cmd->command_arg, env_bis);
}

int	count_cd_args(char *command_arg)
{
	int		count;
	char	**args;

	args = ft_split(command_arg, ' ');
	count = 0;
	while (args && args[count])
		count++;
	ft_free_tab(args);
	return (count);
}

void	execve_builtin_cd(t_command *cmd, t_env *env_bis)
{
	if ((ft_strcmp_minishell(cmd->command, "cd") == 0
			|| ft_strncmp(cmd->command, "cd ", 3) == 0)
			&& (cmd->nb_pipes == 0))
	{
		if (count_cd_args(cmd->command) > 2)
			ft_putstr_fd("cd: too many arguments\n", 2);
		else
			ft_builtin_cd(cmd->command_arg, env_bis);
	}
}
