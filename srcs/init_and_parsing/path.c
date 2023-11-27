/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 14:37:03 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/27 11:31:36 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_get_paths(t_env *env)
{
	char	*path;
	char	**all_paths;
	int		i;

	i = 0;
	path = NULL;
	all_paths = NULL;
	while (env->cpy_env[i])
	{
		if (ft_strncmp_minishell("PATH=", env->cpy_env[i], 5) == 0)
		{
			path = &((env->cpy_env[i])[5]);
			break ;
		}
		i++;
	}
	if (path == NULL)
		return (NULL);
	all_paths = ft_split(path, ':');
	return (all_paths);
}

char	*ft_check_absolute_path(char *args)
{
	if (ft_strchr_slash(args, '/') == 1)
	{
		if (access(args, F_OK | X_OK) == 0)
			return (ft_strdup(args));
		else
			return (NULL);
	}
	return (NULL);
}

char	*find_valid_path(char **temp_path, char *args)
{
	char	*valid_path;
	int		i;

	valid_path = NULL;
	i = 0;
	while (temp_path[i] && !valid_path)
	{
		valid_path = ft_strjoin_minishell(temp_path[i], args);
		if (access(valid_path, F_OK | X_OK) != 0)
		{
			free(valid_path);
			valid_path = NULL;
		}
		i++;
	}
	return (valid_path);
}

char	*ft_check_relative_paths(t_env *env, char *args)
{
	char	**temp_path;
	char	*valid_path;

	temp_path = ft_get_paths(env);
	if ((temp_path == NULL || (temp_path[0][0]) == 0)
		&& (ft_strcmp_minishell(args, "unset") != 0)
		&& (ft_strcmp_minishell(args, "export") != 0)
		&& (ft_strcmp_minishell(args, "pwd") != 0)
		&& (ft_strcmp_minishell(args, "echo") != 0))
	{
		print_no_file_or_directory(env, args);
		ft_free_tab(temp_path);
		return (NULL);
	}
	env->flag_error = false;
	valid_path = find_valid_path(temp_path, args);
	ft_free_tab(temp_path);
	// printf("**************** valid_path 1 = %s \n", valid_path);
	if (valid_path != NULL)
	{
		if (access(valid_path, F_OK | X_OK) == 0)
		{
			// printf("**************** JE SUIS LA \n");
			return (valid_path);
		}
	}
	// printf("**************** JE VAIS free \n");
	free(valid_path); // test
	return (NULL);
}

char	*ft_check_paths(t_env *env, char *args)
{
	char	*valid_path;

	valid_path = ft_check_absolute_path(args);
	// printf("**************** valid_path 2 = %s \n", valid_path);
	if (valid_path != NULL)
		return (valid_path);
	valid_path = ft_check_relative_paths(env, args);
	
	return (valid_path);
}
