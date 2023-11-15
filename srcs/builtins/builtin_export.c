/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/15 17:16:56 by bfresque         ###   ########.fr       */
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
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, start, len);
	var_name[len] = '\0';
	return (var_name);
}

void	check_invalid_var(t_env *env, char *str)
{
	char	*var_name;
	int		i;
	int		j;

	j = 0;
	if (str[0] == '$')
	{
		var_name = extract_var_name(str);
		i = find_env_var(env, var_name);
		if (i != -1)
		{
			ft_putstr_fd("minishell: export: `", 1);
			while (env->cpy_env[i][j] && env->cpy_env[i][j] != '=')
				j++;
			j++;
			while (env->cpy_env[i][j])
				ft_putchar_fd(env->cpy_env[i][j++], 1);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else
			print_env_vars(env);
		free(var_name);
	}
	return ;
}

int	process_arg(char *arg, t_env *env, int *i)
{
	t_export	*export;
	char		*str;
	char		*var_name;

	// str = NULL;
	// str = malloc(sizeof(char) * SIZE);
	// str = malloc(sizeof(char) * ft_strlen(arg));
	if ((check_before_equal(arg) == 0) && (check_after_equal(arg) == 0))
	{
		str = ft_strdup(arg);
		str = handle_quotes_export(arg);
		export = init_export();
		var_name = extract_var_name(str);
		if (var_name)
			export_expander(export, str, env);// str uninitialised
		else
		{
			update_var_env(env, str);
			add_var_env(env, *i, str);
		}
		free(var_name);
		// free(str); // fait SEG/bug apres le ctrl^d pour quitter ./minishell
	}
	else
		return (check_invalid_var(env, arg), g_exit_status);
	free_export(export);
	return (0); //24 lines
}

int	ft_builtin_export(char **args, t_env *env)
{
	int	arg_idx;
	int	i;

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
