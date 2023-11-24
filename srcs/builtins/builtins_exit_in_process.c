/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_in_process.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 15:41:02 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/24 13:53:56 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	validate_exit_status_process(char *exit_status_str)
{
	int	i;

	i = 0;
	if (ft_strcmp_minishell(exit_status_str, "-") == 0)
	{
		ft_putstr_fd("minishell: exit: -: numeric argument required\n", 2);
		return (g_exit_status = 2);
	}
	while (exit_status_str[i])
	{
		if (!(ft_isdigit(exit_status_str[i])) && exit_status_str[i] != '-')
		{
			ft_putstr_fd("minishell: exit: ", 2);
			write(2, exit_status_str, ft_strlen(exit_status_str));
			ft_putstr_fd(": numeric argument required\n", 2);
			return (g_exit_status = 2);
		}
		i++;
	}
	return (ft_atoi(exit_status_str));
}

static char	**parse_exit_args_process(char *input, int *arg_count)
{
	char	*exit_status_str;
	char	**args;

	exit_status_str = ft_strtrim(input + 4, " ");
	args = ft_split(exit_status_str, ' ');
	*arg_count = 0;
	while (args[*arg_count] != NULL)
		(*arg_count)++;
	free(exit_status_str);
	return (args);
}

static void	handle_exit_with_status_process(t_process_data *data,
t_env *env, char *input)
{
	int		arg_count;
	char	**args;
	int		nb_pipe;
	char	*str;

	args = parse_exit_args_process(input, &arg_count);
	nb_pipe = count_pipe(input);
	if (arg_count > 1)
		verif_nb_args_exit();
	else if (nb_pipe > 0 && verif_nb_pipe_exit(args) == 0)
		return ;
	else
	{
		str = epurstr(args[0]);
		g_exit_status = validate_exit_status_process(str);
		free(input);
		free(str);
		ft_free_tab(args);
		ft_free_tab(data->command->command_arg_main); // test, if ? 
		free_child(data, env);
		exit(g_exit_status);
	}
	ft_free_tab(args);
}

int	ft_builtin_write_exit_process(t_process_data *data, t_env *env)
{
	char	*str;

	str = ft_strtrim(data->current->command, " ");
	if (ft_strcmp_minishell(str, "exit") == 0)
	{
		free(str);
		return (1);
	}
	if (ft_strncmp(str, "exit", 4) == 0)
	{
		handle_exit_with_status_process(data, env, str);
		free(str);
		return (1);
	}
	free(str);
	return (0);
}
