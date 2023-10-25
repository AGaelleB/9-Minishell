/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_pipes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:08:08 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/25 17:39:21 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_pipe_at_start(char *input)
{
	int		i;

	i = 0;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|')
	{
		if (input[i + 1] == '|')
			return (2);
		return (1); // Erreur: commande commence avec '|'
	}
	return (0);
}

int	check_pipe_sequence(char *input)
{
	int		pipe_found;
	int		i;

	pipe_found = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			if (pipe_found)
				return (2); // Erreur: "||" trouvé
			pipe_found = 1;
		}
		else if (input[i] != ' ')
			pipe_found = 0;
		i++;
	}
	return (0);
}

int	check_pipe_at_end(char *input)
{
	char	*ptr;

	ptr = input + ft_strlen(input) - 1;
	while (ptr >= input && *ptr == ' ')
		ptr--;
	if (ptr >= input && *ptr == '|')
		return (1); // Erreur: commande finit avec '|'
	return (0);
}

int	check_syntax_errors(char *input)
{
	int	pipe_at_start;
	int	pipe_sequence;

	pipe_at_start = check_pipe_at_start(input);
	pipe_sequence = check_pipe_sequence(input);
	if (pipe_sequence)
		return (pipe_sequence);
	if (pipe_at_start)
		return (pipe_at_start);
	return (check_pipe_at_end(input));
}

int	pipe_syntax_errors(char *input)
{
	int	syntax_error_code;

	syntax_error_code = check_syntax_errors(input);
	if (syntax_error_code == 1)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", 2);
		free(input);
		return (-1);
	}
	else if (syntax_error_code == 2)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '||'\n", 2);
		free(input);
		return (-1);
	}
	return (0);
}
