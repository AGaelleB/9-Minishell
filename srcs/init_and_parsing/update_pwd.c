/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:26:35 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/15 17:26:47 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	update_env_pwd(t_env *env, char *new_pwd)
{
	env->pwd_index = find_env_var(env, "PWD");
	if (env->pwd_index != -1)
	{
		free(env->cpy_env[env->pwd_index]);
		env->cpy_env[env->pwd_index] = ft_strjoin("PWD=", new_pwd);
	}
}

void	update_env_oldpwd(t_env *env) // a tester
{
	char	*current_pwd;

	env->oldpwd_index = find_env_var(env, "OLDPWD");
	current_pwd = getcwd(NULL, 0);
	if (env->oldpwd_index != -1 && current_pwd)
	{
		free(env->cpy_env[env->oldpwd_index]);
		env->cpy_env[env->oldpwd_index] = ft_strjoin("OLDPWD=", current_pwd);
	}
	free(current_pwd);
}
