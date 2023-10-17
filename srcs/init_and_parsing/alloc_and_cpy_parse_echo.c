/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/10/17 14:26:43 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


/* extention of ft_allocate_and_copy*/
void	handle_quotes_echo(char *input, int *i,
	bool *double_quote, bool *single_quote)
{
	if (input[*i] == '\"' && !*single_quote)
	{
		*double_quote = !*double_quote;
		(*i)++;
	}
	else if (input[*i] == '\'' && !*double_quote)
	{
		*single_quote = !*single_quote;
		(*i)++;
	}
}

void	ft_skip_redirection_and_file(char *input, int *i)
{
	if (input[*i] == '>' || input[*i] == '<')
	{
		(*i)++;
		skip_spaces_echo(input, i);
		while (input[*i] && input[*i] != ' ' && input[*i] != '>'
			&& input[*i] != '<')
		{
			if (input[*i] == '\'' || input[*i] == '\"')
				break ;
			(*i)++;
		}
		skip_spaces_echo(input, i);
	}
}

	/* 
	si on est sur un caractere $ et que on est pas dans une simple quote
	
	on va chercher la var de l'env correspondant
	
	si existant
		on stock dans une str a partir du =
		on passe largument sur lequel on est ($blabla) jusqua un ' '(espae)
		et on imprime
	
	si non exitant 
		on imprime telquel le mot ($blabla)
	*/

char *env_var_exists(t_env *env, char *var)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (env->cpy_env[i])
	{
		if (ft_strncmp(env->cpy_env[i], var, ft_strlen(var)) == 0
			&& env->cpy_env[i][ft_strlen(var)] == '=')
		{
			j = ft_strlen(var) + 1; // +1 pour sauter le '='
			str = ft_strdup(&env->cpy_env[i][j]);
			if (!str)
				return (NULL);
			return (str);  // La variable existe et sa valeur est retournée
		}
		i++;
	}
	return (NULL);  // La variable n'existe pas
}

/*Fonction 2: Allocation de l'argument et copie des caractères*/
char	*ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(sizeof(input) * 1000); //WARNING malloc FAUX
	if (!(arg))
		exit(EXIT_FAILURE); // Handle memory allocation failure
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if ((input[*i] == '>' || input[*i] == '<') && !double_quote
			&& !single_quote)
			break ;
		else if (input[*i] == ' ' && !double_quote && !single_quote)
			break ;
		else if (input[*i] == '$' && !single_quote)
		{
			char *str;
			int j = 0;
			// if (input[*i] == '\"') //WARNING quote double FAUX
			// 	(*i)++;
			int start = *i + 1; // +1 pour sauter le caractère '$'
			while (input[*i] && input[*i] != ' ' && input[*i] != '\'' && input[*i] != '\"')
			{
				(*i)++;
				if(input[*i] != '\"' && input[*i] != '\'')
					j++;
			}
			str = ft_substr(input, start, j); // Utilisez ft_substr pour extraire la variable de l'environnement
			if (str)
			{
				char *value = env_var_exists(env, str);
				j = 0;
				if (value)
				{
					while (value[j])
					{
						arg[(*arg_idx)++] = value[j];
						j++;
					}
					free(value);
				}
				free(str);
			}
		}

		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	ft_skip_redirection_and_file(input, i);
	return (arg);
}
