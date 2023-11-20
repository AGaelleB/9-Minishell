/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 15:41:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 14:51:44 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	validate_exit_status(char *exit_status_str)
{
	int	i;

	i = 0;
	if (ft_strcmp_minishell(exit_status_str, "-") == 0)
	{
		// write(2, "exit\n", 5);
		ft_putstr_fd("minishell: exit: -: numeric argument required\n", 2);
		exit(g_exit_status = 2);
	}
	if (exit_status_str[i] == '-' || exit_status_str[i] == '+')
		i = 1;
	while (exit_status_str[i] && exit_status_str[i] != '|')
	{
		if (!(ft_isdigit(exit_status_str[i])))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(exit_status_str, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(g_exit_status = 2);
		}
		i++;
	}
	return (ft_atoi(exit_status_str));
}

static char	**parse_exit_args(char *input, int *arg_count)
{
	char	*exit_status_str;
	char	**args;

	exit_status_str = ft_strtrim(input + 4, " ");
	args = ft_split(exit_status_str, ' ');
	*arg_count = 0;
	while (args[*arg_count] != NULL && args[*arg_count][0] != '|')
		(*arg_count)++;
	free(exit_status_str);
	return (args);
}


static void	handle_exit_with_status(char *input)
{
	int		arg_count;
	char	**args;
	int		i;
	int		exit_status;
	char	*str;

	args = parse_exit_args(input, &arg_count);
	i = 0;
	if (arg_count > 1)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_exit_status = 1;
		return ;
	}
	else
	{
		str = epurstr(args[0]);
		write(2, "exit\n", 6);
		exit_status = validate_exit_status(str);
		g_exit_status = exit_status;
		free(str);
		free(input);
		exit(g_exit_status);
	}
	while (args[i] != NULL)
		free(args[i++]);
	free(args);
}

int	ft_builtin_write_exit(char *input)
{
	char	*str;
	int		nb_pipe;

	str = ft_strtrim(input, " ");
	if (ft_strcmp_minishell(str, "exit") == 0)
	{
		write_exit_simple();
		free(str);
		return (0);
	}
	if (ft_strncmp(str, "exit", 4) == 0)
	{
		nb_pipe = count_pipe(input);
		if (nb_pipe == 0)
		{
			handle_exit_with_status(str);
			free(str);
			return (g_exit_status);
		}
	}
	free(str);
	return (0);
}
