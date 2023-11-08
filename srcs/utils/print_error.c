/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 17:51:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 17:52:31 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_no_file_or_directory(t_env *env, char *args)
{
	write(2, "minishell: ", 12);
	write(2, args, ft_strlen(args));
	write(2, ": No such file or directory", 28);
	write(2, "\n", 1);
	env->flag_error = true;
	g_exit_status = 127;
}

void	ft_print_error(char *str)
{
	int	len;

	if (!ft_strncmp(str, "cd", ft_strlen(str)))
		return ;
	len = ft_strlen(str);
	write(2, "command not found :", 19);
	write(2, str, len);
	write(2, "\n", 1);
}

void	print_error_cd(t_env *env, int i)
{
	if (i == 1)
	{
		write(2, "minishell: cd: ", 15);
		write(2, env->path_to_change, ft_strlen(env->path_to_change));
		write(2, ": ", 2);
		perror("");
		g_exit_status = 1;
	}
	if (i == 2)
	{
		update_env_pwd(env, env->new_directory);
		g_exit_status = 0;
		free(env->new_directory);
	}
}