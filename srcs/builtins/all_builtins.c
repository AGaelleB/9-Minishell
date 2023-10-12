/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:02:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/12 11:11:03 by bfresque         ###   ########.fr       */
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

char	**put_args_in_tab(t_command *current)
{
	t_token *token;
	char **tab;
	int i;

	i = 0;
	tab = malloc(sizeof(char) * (ft_strlen(current->command) + 1));
	if (!tab)
		return (NULL);
	token = current->token_head;
	while (token)
	{
		// printf("IN FONTION TAB = value : %s | type: %d\n", token->split_value, token->type);
		if (token->type == 1)
		{
			tab[i] = token->split_value;
			i++;
		}
		token = token->next;
	}
	tab[i] = '\0';
	return (tab);
}

void	ft_all_builtins_verif(t_command *current)
{
	int	cmd_count;

	cmd_count = 0;
	while (current)
	{
		// printf("%sCommand %d: %s%s\n", GREEN, cmd_count, current->command, RESET);
		if (ft_strncmp(current->command, "pwd", 3) == 0)
		{
			ft_builtin_pwd_fd(STDOUT_FILENO);
			exit (0);
		}
		if (ft_strncmp(current->command, "echo", 4) == 0)
		{
			if (current->command_path != NULL)
			{
				char **tab = put_args_in_tab(current);
				// int i = 0;
				// while(tab[i])
				// {
				// 	printf("tab[%d] : %s\n", i, tab[i]);
				// 	i++;
				// }
				ft_builtin_echo_fd(tab);
				exit (0);
			}
		}
		cmd_count++;
		current = current->next;
		// printf("\n");  // Just to separate different commands
	}
}
