/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 14:48:37 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/28 15:09:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_pwd()
{
	char	*pwd;

	printf("ft_builtin_pwd\n");
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (-1);
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int	ft_builtin_pwd_fd(int fd)
{
	char	*pwd;

	printf("ft_builtin_pwd_fd\n");
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (-1);
	ft_putstr_fd(pwd, fd);
	ft_putchar_fd('\n', fd);
	free(pwd);
	return (0);
}
