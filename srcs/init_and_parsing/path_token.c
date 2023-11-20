/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 11:54:09 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/20 15:54:28 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**ft_get_paths_token(t_env *env)
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
	all_paths = ft_split(path, ':'); // casse tout quand on export avec : path=path:pwd plusieurs fois
	return (all_paths);
}

char	*ft_check_absolute_path_token(char *args)
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

char	*find_valid_path_token(char **temp_path, char *args)
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

char	*ft_check_relative_paths_token(t_env *env, char *args)
{
	char	**temp_path;
	char	*valid_path;

	temp_path = ft_get_paths_token(env);
	if (temp_path == NULL)
		return (NULL);
	if (temp_path[0][0] == 0) //ne pas remttre au dessus (segfault)
		return (NULL);
	valid_path = find_valid_path_token(temp_path, args);
	ft_free_tab(temp_path);
	if (valid_path != NULL)
	{
		if (access(valid_path, F_OK | X_OK) == 0)
			return (valid_path);
	}
	return (NULL);
}

char	*ft_check_paths_token(t_env *env, char *args)
{
	char	*valid_path;

	valid_path = ft_check_absolute_path_token(args);
	if (valid_path != NULL)
		return (valid_path);
	valid_path = ft_check_relative_paths_token(env, args);
	return (valid_path);
}
