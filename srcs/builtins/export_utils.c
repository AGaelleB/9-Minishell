/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 10:16:16 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/20 10:51:51 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*check_none_var(char *str)
{
	char	*str_cpy;
	int		i;
	int		y;

	i = 0;
	y = 0;
	str_cpy = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!str_cpy)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_')) //MODIF ICI
				i++;
		}
		str_cpy[y++] = str[i++];
	}
	return (str_cpy);
}

int	expand_variable(t_export *export, char **str, t_env *env)
{
	char	*var_name;

	while (1)
	{
		export->k = 0;
		var_name = extract_var_name(*str);
		export->j = find_env_var(env, var_name);
		if (export->j != -1)
		{
			if (!(export->ret))
			{
				export->ret = malloc(sizeof(*str)
				* (ft_strlen(env->cpy_env[export->j]) + ft_strlen(*str) + 2));
			}
			begin_var(export, *str);
			copy_env_in_return(export, env);
			*str = re_init_var_str(export, *str);
			if (!(*str))
				return (free(var_name), 0);
		}
		else
		{
			*str = check_none_var(*str);
			add_var_env(env, export->i, *str);
			free(var_name);
			return (1);
		}
		free(var_name);
	}
}

int	export_expander(t_export *export, char *str, t_env *env)
{
	char	*str_cpy;

	str_cpy = ft_strdup(str);
	if (expand_variable(export, &str, env) == 0) // pb du fait qu'on renvoie l'adresse ? &str
	{
		update_var_env(env, str_cpy);
		export->ret[export->l] = '\0';
		add_var_env(env, export->i, export->ret);
		free(str_cpy);
		free(str);
		return (g_exit_status);
	}
	free(str_cpy); // NEW FREE
	return (g_exit_status);
}

/* int	expand_variable(t_export *export, char *str, t_env *env)
{
	char	*var_name;

	while (1)
	{
		export->k = 0;
		var_name = extract_var_name(str);
		export->j = find_env_var(env, var_name);
		if (export->j != -1)
		{
			begin_var(export, str);
			copy_env_in_return(export, env);
			str = re_init_var_str(export, str);
			if (!(str))
				return (free(var_name), 0);
		}
		else
		{
			str = check_none_var(str);
			add_var_env(env, export->i, str);
			free(var_name);
			return (1);
		}
		free(var_name);
	}
} */
