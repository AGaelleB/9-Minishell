/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:23:27 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 10:11:54 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	write_exit_simple(void)
{
	g_exit_status = 0;
	write(1, "exit\n", 5);
	exit(g_exit_status);
}

int	ft_builtin_enter(char *input)
{
	if (ft_strcmp_minishell(input, "") == 0)
	{
		free(input);
		return (1);
	}
	return (0);
}

int	ft_is_all_space(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (!(input[i] >= 9 && input[i] <= 13) && input[i] != 32)
			return (0);
		i++;
	}
	// free(input);
	return (1);
}

void	ft_builtin_ctrl_d(char *input, t_command *new_cmd,
	t_env *env_bis, int flag_ok)
{
	if (!input)
	{
		write(1, "exit", 5);
		write(1, "\n", 1);
		ft_close_all_fd();
		ft_free_env(env_bis);
		if (flag_ok == 1)
			ft_free_all(new_cmd, new_cmd->token_head);
		exit(0);
	}
}
