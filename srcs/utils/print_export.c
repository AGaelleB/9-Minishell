/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:16:44 by bfresque          #+#    #+#             */
/*   Updated: 2023/12/08 12:18:00 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export(char *str, int fd)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (str[i])
	{
		ft_putchar_fd(str[i], fd);
		if (str[i] == '=' && flag == 0)
		{
			flag = 1;
			ft_putchar_fd('\"', fd);
		}
		i++;
	}
	if (flag == 1)
		ft_putchar_fd('\"', fd);
}

int	print_env_vars(t_env *env)
{
	int	i;

	i = 0;
	while (env->cpy_env[i])
	{
		ft_putstr_fd("export ", STDOUT_FILENO);
		print_export(env->cpy_env[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (1);
}

void	print_expander_cmd(t_env *env, char *str, int i)
{
	int		j;
	int		k;

	j = 0;
	k = 0;
	while (env->cpy_env[i][j] && env->cpy_env[i][j] != '=')
		j++;
	j++;
	while (env->cpy_env[i][j])
		str[k++] = env->cpy_env[i][j++];
	write(1, "minishell: ", 11);
	ft_putstr_fd(str, 1);
	write(1, ": command not found\n", 21);
}

void	skip_var_name(char *str, int *i)
{
	(*i)++;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
}
