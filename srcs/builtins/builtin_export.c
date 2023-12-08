/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/08 16:03:38 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*extract_var_name(char *str)
{
	char	*start;
	char	*var_name;
	int		len;

	start = ft_strchr(str, '$');
	if (!start)
		return (NULL);
	start++;
	len = 0;
	while (start[len] && ((ft_isalnum(start[len]) || start[len] == '_')
			&& start[len] != ':' && start[len] != '$'))
		len++;
	var_name = malloc(sizeof(int) * (len + 1));
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	return (var_name);
}

int	process_arg(t_export *export, char *arg, t_env *env, int *i)
{
	char	*var_name;

	export->str = NULL;
	if ((check_before_equal(arg) == 0) && (check_after_equal(arg) == 0))
	{
		export->str = handle_quotes_export(arg);
		export = init_export(export);
		var_name = extract_var_name(export->str);
		if (var_name)
		{
			free(var_name);
			export_expander(export, export->str, env);
		}
		else
		{
			update_var_env(env, export->str);
			add_var_env(env, *i, export->str);
		}
	}
	else
		return (check_invalid_var(env, arg), g_exit_status);
	return (0);
}

int	handle_export_args(char **args, t_env *env, t_export *export)
{
	int	arg_idx;
	int	i;

	i = 0;
	arg_idx = 1;
	while (args[arg_idx])
	{
		if (process_arg(export, args[arg_idx], env, &i) != 0)
		{
			free_export_str(export);
			return (g_exit_status);
		}
		arg_idx++;
	}
	return (g_exit_status);
}

int	ft_builtin_export(char **args, t_env *env)
{
	t_export	*export;

	export = malloc(sizeof(t_export));
	if (!export)
		return (0);
	export->new = NULL;
	export->ret = NULL;
	if (!args[1])
	{
		free(export);
		return (print_env_vars(env));
	}
	if (handle_export_args(args, env, export) == 1)
		return (0);
	if (export->str && ft_strchr(export->str, '$'))
		free_export_str(export);
	else
		free_export_basic(export);
	return (g_exit_status);
}
