/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bis.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:20:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/01 16:11:56 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

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

void	ft_free_all(t_command *current, t_token *token)
{
	ft_close_all_fd();
	if (token)
		ft_free_token(current);
	if (current)
		ft_free_current(current);
	(void)token;
}

void	cleanup(pid_t *child_pids, int infile)
{
	free(child_pids);
	if (infile != 0)
		close(infile);
}
