/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_invalid_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 12:11:23 by bfresque          #+#    #+#             */
/*   Updated: 2023/12/08 12:13:23 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_env_var(t_env *env, char *str)
{
	char	*var_name;
	int		i;
	int		j;

	j = 0;
	if (str[0] == '$')
	{
		var_name = extract_var_name(str);
		i = find_env_var(env, var_name);
		if (i != -1)
		{
			while (env->cpy_env[i][j] && env->cpy_env[i][j] != '=')
				j++;
			j++;
			while (env->cpy_env[i][j])
				ft_putchar_fd(env->cpy_env[i][j++], 1);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else
			print_env_vars(env);
		free(var_name);
	}
}

void	update_string_env(t_env *env, char *str, int i, int j)
{
	char	*test;
	char	*exit_stat;
	int		h;

	test = malloc(sizeof(char) * (ft_strlen(str) + 11));
	if (!test)
		return ;
	h = 0;
	ft_memset(test, '\0', ft_strlen(str) + 11);
	while (str[j])
	{
		if (str[j] == '$' && str[j + 1] == '?')
		{
			i = 0;
			exit_stat = ft_itoa(g_exit_status);
			while (exit_stat[i])
				test[h++] = exit_stat[i++];
			j += 2;
			free(exit_stat);
		}
		else
			test[h++] = str[j++];
	}
	update_var_env(env, test);
	add_var_env(env, 0, test);
}

void	check_invalid_var(t_env *env, char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (str[0] == '$')
		process_env_var(env, str);
	else
		update_string_env(env, str, i, j);
}
