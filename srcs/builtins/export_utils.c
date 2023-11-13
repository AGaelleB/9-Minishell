/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 10:16:16 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/13 11:50:45 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_export	*init_export(void)
{
	t_export	*export;

	export = malloc(sizeof(t_export));
	if (!export)
		return (NULL);
	export->ret = malloc(sizeof(char) * SIZE);
	if (!export->ret)
		return (NULL);
	export->new = malloc(sizeof(char) * SIZE);
	if (!export->new)
		return (NULL);
	export->flag = 0;
	export->i = 0;
	export->j = 0;
	export->k = 0;
	export->l = 0;
	export->m = 0;
	return (export);
}

void	begin_var(t_export *export, char *str)
{
	if (export->flag == 0)
	{
		export->flag = 1;
		while (str[export->i] && str[export->i] != '=')
			export->ret[export->l++] = str[export->i++];
		while (str[export->i] && str[export->i] != '$')
			export->ret[export->l++] = str[export->i++];
	}
}

void	copy_env_in_return(t_export *export, t_env *env)
{
	while (env->cpy_env[export->j][export->k] != '=')
		export->k++;
	if (env->cpy_env[export->j][export->k] == '=')
		export->k++;
	while (env->cpy_env[export->j][export->k])
	{
		export->ret[export->l] = env->cpy_env[export->j][export->k++];
		export->l++;
	}
	export->i++;
}

void	re_init_var_str(t_export *export, char *str)
{
	while (ft_isalpha(str[export->i]))
		export->i++;
	while (str[export->i] && str[export->i] != '$')
			export->ret[export->l++] = str[export->i++];
	export->k = 0;
	export->m = export->i;
	while (str[export->i])
	{
		export->new[export->k] = str[export->i++];
		export->k++;
	}
	export->i = export->m;
	export->new[export->k++] = '\0';
}

int	export_expander(t_export *export, char *var_name, char *str, t_env *env)
{
	while (str[export->i])
	{
		export->k = 0;
		var_name = extract_var_name(str);
		export->j = find_env_var(env, var_name);
		if (export->j != -1)
		{
			begin_var(export, str);
			copy_env_in_return(export, env);
			re_init_var_str(export, str);
		}
		else
		{
			add_var_env(env, export->i, str);
			free (var_name);
			// faire free de la struct export
			return (g_exit_status);
		}
	}
	export->ret[export->l] = '\0';
	add_var_env(env, export->i, export->ret);
	free(str);
	free (var_name);
	// faire free de la struct export
	return (g_exit_status);
}
