/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 16:42:01 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_tokens(char **tokens, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char	**free_token_split(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
		free(tokens[i++]);
	free(tokens);
	return (NULL);
}

void ft_free_env(t_env *env)
{
	int	i;

	i = 0;
	// printf("%sje commance a free env\n%s", BLUE, RESET);
	while (env->cpy_env[i])
	{
		free(env->cpy_env[i]);
		i++;
	}
	free(env->cpy_env);
	free(env);
}

void ft_free_all(t_command *current, t_token *token)
{
	if (token)
		ft_free_token(current);
	if (current)
		ft_free_current(current);
}
