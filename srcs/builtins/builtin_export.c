/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/17 11:10:32 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*create_str_from_arg(char *arg)
{
	int		i;
	char	*str;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, arg, i + 1);
	return (str);
}

int	update_var_env(t_env *env, char *arg)
{
	int		i;
	char	*str;

	str = create_str_from_arg(arg);
	if (!str)
		return (-1);
	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], str, ft_strlen(str)) == 0
			&& env->cpy_env[i][ft_strlen(str)] == '=')
		{
			free(env->cpy_env[i]);
			env->cpy_env[i] = ft_strdup(arg);
			env->cpy_env[i + 1] = NULL;
			free(str);
			return (1);
		}
		i++;
	}
	free(str);
	return (0);
}

int	add_var_env(t_env *env, char **args, int i, int arg_idx)
{
	i = 0;
	arg_idx = 1;
	while (env->cpy_env[i])
	{
		if (env->cpy_env[i + 1] == NULL)
		{
			printf("arg : %s\n", args[arg_idx]);
			env->cpy_env[i + 1] = args[arg_idx];
			env->cpy_env[i + 2] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_builtin_export(char **args, t_env *env)
{
	int	arg_idx;
	int	i;

	i = 0;
	env->flag_builtin = true;
	if (!args[1])
	{
		while (env->cpy_env[i])
		{
			ft_putstr_fd("export ", STDOUT_FILENO);
			ft_putstr_fd(env->cpy_env[i], STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
			i++;
		}
		return (1);
	}
	arg_idx = 1;
	while (args[arg_idx])
	{
		if (update_var_env(env, args[arg_idx]) == 1)
			return (1);
		arg_idx++;
	}
	add_var_env(env, args, i, arg_idx);
	return (0);
}
