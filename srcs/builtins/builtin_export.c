/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/14 11:39:45 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_export(char *str, int fd)
{
	int	i;
	int	flag;

	flag = 0;
	i = 0;
	while (str[i])
	{
		ft_putchar_fd(str[i], fd);
		if (str[i] == '=' && flag == 0)
		{
			flag = 1;
			ft_putchar_fd('\"', fd);
		}
		i++;
	}
	if (flag == 1)
		ft_putchar_fd('\"', fd);
}

static int	print_env_vars(t_env *env)
{
	int	i;

	i = 0;
	while (env->cpy_env[i])
	{
		ft_putstr_fd("export ", STDOUT_FILENO);
		print_export(env->cpy_env[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (1);
}

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
	while (start[len]
		&& (ft_isalnum(start[len]) && start[len] != '_'
			&& start[len] != ':' && start[len] != '$'))
		len++;
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	return (var_name);
}

int	process_arg(char *arg, t_env *env, int *i)
{
	t_export	*export;
	char		*str;
	char		*var_name;

	if ((check_before_equal(arg) == 0) && (check_after_equal(arg) == 0))
	{
		str = handle_quotes_export(arg);
		export = init_export();
		var_name = extract_var_name(str);
		if (var_name)
			export_expander(export, str, env);
		else
		{
			update_var_env(env, str);
			add_var_env(env, *i, str);
		}
		// free(var_name);
		// free(str);
	}
	else
		return (g_exit_status);
	return (0);
	
}

int	ft_builtin_export(char **args, t_env *env)
{
	int			arg_idx;
	int			i;

	i = 0;
	if (!args[1])
		return (print_env_vars(env));
	arg_idx = 1;
	while (args[arg_idx])
	{
		if (process_arg(args[arg_idx], env, &i) != 0)
			return (g_exit_status);
		arg_idx++;
	}
	// free(str);
	// free(ret);
	return (g_exit_status);
}
