/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_export_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 15:47:32 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/01 11:59:03 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_export	*init_export(t_export *export)
{
	export->ret = NULL;
	export->new = malloc(sizeof(char) * SIZE);
	if (!export->new)
		return (NULL);
	ft_memset(export->new, '\0', SIZE);
	export->flag = 0;
	export->i = 0;
	export->j = 0;
	export->k = 0;
	export->l = 0;
	export->m = 0;
	return (export);
}

char	*re_init_var_str(t_export *export, char *str)
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
	if (ft_is_all_space(export->new))
		return (NULL);
	return (export->new);
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
		export->ret[export->l++] = env->cpy_env[export->j][export->k++];
	export->i++;
}
