/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:21:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/17 16:03:37 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	exit_with_error(char *message, pid_t *child_pids)
{
	perror(message);
	free(child_pids);
	exit(1);
}

void	print_error_cd(t_env *env, int i)
{
	if (i == 1)
	{
		write(2, "minishell: cd: ", 15);
		write(2, env->path_to_change, ft_strlen(env->path_to_change));
		write(2, ": ", 2);
		perror("");
	}
	if (i == 2)
	{
		update_env_pwd(env, env->new_directory);
		free(env->new_directory);
	}
}

/* int	check_valid_identifier(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == '\'' || c == '\"' || c == ',' || c == '.'
		|| c == ':' || c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~' || c == ';'
		|| c == '=' || c == '-' || c == '?' || c == '&' || c == '*') //|| c == ' '
	{
		return (1);
	}
	else
		return (0);
} */
