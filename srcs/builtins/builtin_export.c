/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/13 14:16:25 by bfresque         ###   ########.fr       */
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

char *extract_var_name(char *str)
{
	char *start;
	char *var_name;
	int len;

	start = ft_strchr(str, '$');
	if (!start)
		return (NULL);
	start++;
	len = 0;
	while (start[len]
		&& (ft_isalnum(start[len]) && start[len] != '_' && start[len] != ':' && start[len] != '$'))
		len++;
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	// printf("%s var_name : %s%s\n",MAGENTA, var_name, RESET);
	return (var_name);
}

// int	check_identifier(char *str)
// {
// 	char	*str_cpy;
// 	int		i;

// 	i = 0;
// 	while(str[i] != '=')
// 		i++;
// 	str_cpy = ft_strdup(str);
// 	printf("%s : str_cpy \n", str_cpy);
// 	return (check_after_equal(str_cpy));
// }

int	ft_builtin_export(char **args, t_env *env)
{
	t_export	*export;
	int			arg_idx;
	int			i;
	char		*str;
	char		*var_name;

	i = 0;
	if (!args[1])
		return (print_env_vars(env));
	arg_idx = 1;
	while (args[arg_idx])
	{
		if ((check_before_equal(args[arg_idx]) == 0)
			&& (check_after_equal(args[arg_idx]) == 0))
		{
			str = handle_quotes_export(args[arg_idx]);
			// printf("str : %s\n", str);
			export = init_export();
			var_name = extract_var_name(str);
			if (var_name)
				export_expander(export, var_name, str, env);
			else
			{
				update_var_env(env, str);
				// printf("ATTENTION derriere on sors\n");
				add_var_env(env, i, str);
			}
		}
		arg_idx++;
	}
	// str = handle_quotes_export(args[arg_idx]);
	
	// free(str);
	// free(ret);
	// if (flag == 1)
	// 	return (g_exit_status = 1);
	return (g_exit_status);
}
