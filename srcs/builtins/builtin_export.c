/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 10:39:49 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_valid_identifier_export(char *str)
{
	int	flag;
	int	i;

	i = 0;
	flag = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] && !flag)
		{
			if (ft_isalnum(str[i]) || str[i] == '_')
				i++;
			else
				flag = 1;
		}
	}
	else
		flag = 1;
	if (flag)
	{
		ft_putstr_fd("minishell: export: \'", 2);
		write(2, &str[i], 1);
		ft_putstr_fd("\': not a valid identifier\n", 2);
		g_exit_status = 1;
		return (g_exit_status);
	}
	return (0);
}

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
			env->cpy_env[i + 1] = args[arg_idx];
			env->cpy_env[i + 2] = NULL; ///
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
		if (check_valid_identifier_export(args[arg_idx]))
		{
			if (update_var_env(env, args[arg_idx]) == 1)
				return (1);
			arg_idx++;
		}
	}
	add_var_env(env, args, i, arg_idx);
	return (g_exit_status);
}
