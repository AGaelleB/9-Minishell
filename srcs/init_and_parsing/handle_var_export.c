/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_var_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:00:41 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/12 13:38:25 by abonnefo         ###   ########.fr       */
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
	printf("PATH str de create : %s\n", str);
	return (str);
}

// static void	remove_env_var(t_env *env, int idx)
// {
// 	free(env->cpy_env[idx]);
// 	while (env->cpy_env[idx + 1])
// 	{
// 		env->cpy_env[idx] = env->cpy_env[idx + 1];
// 		idx++;
// 	}
// 	env->cpy_env[idx] = NULL;
// }

// char	*check_var(char *arg)
// {
// 	int		i;
// 	char	*str;

// 	i = 0;
// 	str = malloc(sizeof(char) * 100);
// 	while (arg[i] && arg[i] != '=')
// 	{
// 		str[i] = arg[i];
// 		i++;
// 	}
// 	printf("PATH str de check : %s\n", str);
// 	return (str);
// }

// int update_var_env(t_env *env, char *arg)
// {
// 	int i;
// 	char *str;
// 	char *str_two;

// 	printf("J'UPDATE PATH\n");
// 	// str_two = check_var(arg);
// 	str = create_str_from_arg(arg);
// 	if (!str)
// 		return (-1);
// 	i = find_env_var(env, str_two);
// 	if (i != -1)
// 		remove_env_var(env, i);
// 	// i = 0;
// 	// while (env->cpy_env[i])
// 	// {
// 	// 	if (ft_strncmp(env->cpy_env[i], str, ft_strlen(str)) == 0
// 	// 		&& env->cpy_env[i][ft_strlen(str)] == '=')
// 	// 	{
// 	// 		free(env->cpy_env[i]);
// 	// 		env->cpy_env[i] = ft_strdup(arg);
// 	// 		env->cpy_env[i + 1] = NULL;
// 	// 		free(str);
// 	// 		return (1);
// 	// 	}
// 	// 	i++;
// 	// }
// 	free(str);
// 	// return (0);
// 	return add_var_env(env, 0, ft_strdup(arg));
// }

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
			// env->cpy_env[i + 1] = NULL;
			free(str);
			return (1);
		}
		i++;
	}
	free(str);
	return (0);
}

int	add_var_env(t_env *env, int i, char *str)
{
	i = 0;
	printf("add_var_env\n");
	while (env->cpy_env[i])
	{
		if (env->cpy_env[i + 1] == NULL)
		{
			env->cpy_env[i + 1] = str;
			env->cpy_env[i + 2] = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
