/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 15:41:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 15:36:20 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
void write_exit_simple()
{
	g_exit_status = 0;
	write(1, "exit\n", 5);
	exit(g_exit_status);
}

int ft_builtin_write_exit(char *input) // NEW
{
	char *str;
	char *exit_status_str;
	int exit_status;
	int i;
	
	i = 0;
	str = ft_strtrim(input, " ");
	if (ft_strcmp_minishell(str, "exit") == 0)
	{
		write_exit_simple();
		free(str);
		return (0);
	}
	if (ft_strncmp(str, "exit", 4) == 0)
	{
		free(str);
		exit_status_str = input + 4;
		if (*exit_status_str == ' ')
		{
			exit_status_str++;
			while(exit_status_str[i])
			{
				if (!(ft_isdigit(exit_status_str[i])))
				{
					write(2, "exit\n", 5);
					ft_putstr_fd("minishell: exit: ", 2);
					write(2, &exit_status_str[0], ft_strlen(exit_status_str));
					ft_putstr_fd (": numeric argument required\n", 2);
					g_exit_status = 2;
					exit(g_exit_status);
				}
				i++;
			}
			exit_status = ft_atoi(exit_status_str);
			g_exit_status = exit_status;
			printf("g_exit_status : %d\n", g_exit_status);
			write(2, "exit\n", 5);
			exit(g_exit_status);
		}
		else
			return (0) ;
	}
	return (0);
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
		if (input[i] != ' ')
			return (0);
		i++;
	}
	free(input);
	return (1);
}

void	ft_builtin_ctrl_d(char *input) // NEW
{
	if (!input)
	{
		// write(1, "\n", 1);
		write(1, "exit", 5);
		write(1, "\n", 1);
		ft_close_all_fd();
		exit(0);
	}
}
