/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 11:57:35 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/12 15:24:49 by abonnefo         ###   ########.fr       */
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
		return NULL;
	start++;
	len = 0;
	while (start[len]
		&& (ft_isalnum(start[len]) && start[len] != '_' && start[len] != ':' && start[len] != '$'))
		len++;
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	strncpy(var_name, start, len); // a faire strcpy
	var_name[len] = '\0';
	printf("%s var_name : %s%s\n",MAGENTA, var_name, RESET);
	return (var_name); // renvoie la varaible a charcher
}

int	ft_builtin_export(char **args, t_env *env)
{
	int		arg_idx;
	int		i;
	char	*str;

	i = 0;
	if (!args[1])
		return (print_env_vars(env));
	arg_idx = 1;
	while (args[arg_idx])
	{
		str = handle_quotes_export(args[arg_idx]);
		if (check_valid_identifier_export(str) == 0)
		{
			if (update_var_env(env, str) == 1)
				return (1);
			arg_idx++;
		}
		else
			break ;
	}
	i = 0;
	
	char *ret;
	char *new;

	// if (ret)
	// 	free(ret);
	new = malloc(sizeof(char) * SIZE);
	ret = malloc(sizeof(char) * SIZE);
	if (!ret)
		return (-1);
	int l = 0;
	new = str;
	int m;
	int flag = 0;
	while(str[i])
	{
		int  k = 0;
		int j = find_env_var(env, extract_var_name(str));
		if (j != -1)
		{
			if (flag == 0)
			{
				flag =1;
				while (str[i] && str[i] != '=')
					ret[l++] = str[i++];
				printf("%sstr[%d] = %c%s\n",YELLOW, i, str[i], RESET);
				while (str[i] && str[i] != '$')
					ret[l++] = str[i++];
				printf("%sstr[%d] = %c%s\n",YELLOW, i, str[i], RESET);
			}
			while (env->cpy_env[j][k] != '=')
				k++;
			k++;
			while(env->cpy_env[j][k])
			{
				// printf("env[%d] = %c\n", k, env->cpy_env[j][k]);
				ret[l] = env->cpy_env[j][k++];
				printf("ret[%d] = %c\n", l, ret[l]);
				l++;
			}
			printf("%sstr[%d] = %c%s\n",BLUE, i, str[i], RESET);
			i++;
			printf("%sstr[%d] = %c%s\n",BLUE, i, str[i], RESET);
			while(str[i] && str[i] != '$')
			{
				if (!str[i])
					break ;
				i++;
				printf("str[%d] = %c\n", i, str[i]);
			}
			k = 0;
			m = i;
			printf("%sstr[%d] = %c\n%s", RED, i, str[i], RESET);
			while(str[i])
			{
				new[k] = str[i++];
				k++;
			}
			printf("%snew[%d] = %c\n%s", MAGENTA, k, new[k], RESET);
			i = m;
			new[k++] = '\0';
		}
		else
		{
			printf("attention derriere on sors\n");
			break ;
			// while(str[i] && str[i] != '$')
			// 	i++;
		}
	}
	ret[l] = '\0';
	add_var_env(env, i, ret);
	free(str);
	// free(ret);
	return (g_exit_status);
}

/* 
int	ft_builtin_export(char **args, t_env *env)
{
	int		arg_idx;
	int		i;
	char	*str;

	i = 0;
	if (!args[1])
		return (print_env_vars(env));
	arg_idx = 1;
	while (args[arg_idx])
	{
		str = handle_quotes_export(args[arg_idx]);
		if (check_valid_identifier_export(str) == 0)
		{
			if (update_var_env(env, str) == 1)
				return (1);
			arg_idx++;
		}
		else
			break ;
	}
	i = 0;
	int  k = 0;
	int j = find_env_var(env, extract_var_name(str));
	printf("%s j : %d%s\n",GREEN, j, RESET);
	if (j != -1)
	{
		while (str[i] != '=')
			i++;
		while (str[i] != '$')
			i++;
		if (str[i] == '$')
		{
			// ici je veux pas stocker le $ dans la str
			i++;
		}
		while (env->cpy_env[j][k] != '=')
			k++;
		if (env->cpy_env[j][k] == '=')
			k++;
		while(env->cpy_env[j][k])
			str[i++] = env->cpy_env[j][k++];
	}
	add_var_env(env, i, str);
	return (g_exit_status);
}
 */