/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 10:16:16 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/13 16:16:48 by abonnefo         ###   ########.fr       */
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
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			while (ft_isalnum(str[i]))
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
			begin_var(export, *str);
			copy_env_in_return(export, env);
			*str = re_init_var_str(export, *str);
			if (!(*str))
				return (free(var_name), 0);
		}
		else
		{
			*str = check_none_var(*str);
			free(var_name);
			return (0);
		}
		free(var_name);
	}
}

int	export_expander(t_export *export, char *var_name, char *str, t_env *env)
{
	char	*str_cpy;

	str_cpy = ft_strdup(str);
	(void)var_name;
	if (expand_variable(export, &str, env) == 0)
	{
		update_var_env(env, str_cpy);
		export->ret[export->l] = '\0';
		add_var_env(env, export->i, export->ret);
		free(str_cpy);
		free(str);
		return (g_exit_status);
	}
	return (g_exit_status);
}
