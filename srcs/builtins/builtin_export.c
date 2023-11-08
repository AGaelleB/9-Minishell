/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 11:36:54 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_valid_identifier_export(char *str)
{
	int	flag;
	int	i;

	i = 0;
	flag = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (str[i] && !flag)
		{
			if (ft_isalnum(str[i]) || str[i] == '_' || str[i] == '='
				|| str[i] == '\'' || str[i] == '\"' || str[i] == ' ')
				i++;
			else
				flag = 1;
		}
	}
	else
		flag = 1;
	if (flag)
	{
		ft_putstr_fd("minishell: export: \'", 2);
		write(2, &str[i], 1);
		ft_putstr_fd("\': not a valid identifier\n", 2);
		g_exit_status = 1;
		return (g_exit_status);
	}
	return (0);
}

char	*create_str_from_arg(char *arg)
{
	int		i;
	char	*str;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	str = malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, arg, i + 1);
	return (str);
}

int	update_var_env(t_env *env, char *arg)
{
	int		i;
	char	*str;

	str = create_str_from_arg(arg);
	if (!str)
		return (-1);
	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], str, ft_strlen(str)) == 0
			&& env->cpy_env[i][ft_strlen(str)] == '=')
		{
			free(env->cpy_env[i]);
			env->cpy_env[i] = ft_strdup(arg);
			env->cpy_env[i + 1] = NULL;
			free(str);
			return (1);
		}
		i++;
	}
	free(str);
	return (0);
}

int	add_var_env(t_env *env, char **args, int i, int arg_idx)
{
	i = 0;
	arg_idx = 1;
	while (env->cpy_env[i])
	{
		if (env->cpy_env[i + 1] == NULL)
		{
			env->cpy_env[i + 1] = args[arg_idx];
			env->cpy_env[i + 2] = NULL; ///
			return (1);
		}
		i++;
	}
	return (0);
}

void	print_export(char *str, int fd)
{
	int	i;
	int flag;

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
	ft_putchar_fd('\"', fd);
}

char *handle_quotes_export(char *str)
{
	int i;
	int y;
	bool single_quote;
	bool double_quote;
	char *dest;

	i = 0;
	y = 0;
	single_quote = false;
	double_quote = false;
	dest = malloc(sizeof(str) * (ft_strlen(str) + 1));
	if (!dest)
		return (NULL);
	while (str[i])
	{
		if (!single_quote && str[i] == '\"' && str[i + 1] == '\"')
			i += 2;
		else if (!double_quote && str[i] == '\'' && str[i + 1] == '\'')
			i += 2;
		if (!double_quote && str[i] == '\'')
			single_quote = !single_quote;
		else if (!single_quote && str[i] == '\"')
			double_quote = !double_quote;
		if (!single_quote && str[i] == '\"')
			i++;
		else if (!double_quote && str[i] == '\'')
			i++;
		dest[y] = str[i];
		i++;
		y++;
	}
	dest[y] = '\0';
	return (dest);
}

int	ft_builtin_export(char **args, t_env *env)
{
	int	arg_idx;
	int	i;
	char *str;

	i = 0;
	if (!args[1])
	{
		while (env->cpy_env[i])
		{
			
			str = handle_quotes_export(env->cpy_env[i]);
			ft_putstr_fd("export ", STDOUT_FILENO);
			print_export(str, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
			i++;
		}
		return (1);
	}
	arg_idx = 1;
	while (args[arg_idx])
	{
		printf("%sact : %s%s\n", MAGENTA, args[arg_idx], RESET);
		if (check_valid_identifier_export(args[arg_idx]))
		{
			if (update_var_env(env, args[arg_idx]) == 1)
				return (1);
			arg_idx++;
		}
		else
			break ;
	}
	add_var_env(env, args, i, arg_idx);
	return (g_exit_status);
}
