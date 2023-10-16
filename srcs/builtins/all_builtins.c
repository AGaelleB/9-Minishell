/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:02:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/16 11:27:19 by bfresque         ###   ########.fr       */
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

int	ft_all_builtins_verif(t_command *current, t_env	*env)
{
	int		cmd_count;
	char	**tab;

	cmd_count = 0;
	while (current)
	{
		if (ft_strcmp_minishell(current->command, "pwd") == 0)
			ft_builtin_pwd_fd(STDOUT_FILENO);
		if (ft_strncmp(current->command, "echo ", 5) == 0)
		{
			tab = parse_input_quote_echo(current->command);
			ft_builtin_echo_fd(tab);
		}
		if (ft_strcmp_minishell(current->command, "env") == 0)
			ft_builtin_env(env);
		cmd_count++;
		current = current->next;
	}
	return(0);
}
