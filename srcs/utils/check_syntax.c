/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:08:08 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/24 09:58:05 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_valid_caractere_filename(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '#'
		|| c == '(' || c == ')' || c == '!' || c == ';'
		|| c == '?' || c == '&' || c == '*' || c == '\\')
	{
		return (1);
	}
	else
		return (0);
}

int	check_syntax_errors(char *input)
{
	char	*ptr;
	int		pipe_found;
	int		i;

	ptr = input;
	pipe_found = 0;
	i = 0;
	while (ptr[i] == ' ')
		i++;
	if (ptr[i] >= input[i] && ptr[i] == '|')
	{
		if (ptr[i + 1] == '|')
			return (2);
		return (1); // Erreur: commande commence avec '|'
	}
	i = 0;
	while (ptr[i])
	{
		if (ptr[i] == '|')
		{
			if (pipe_found)
				return (2); // Erreur: "||" trouvé
			pipe_found = 1;
		}
		else if (ptr[i] != ' ')
			pipe_found = 0;
		i++;
	}
	ptr = input + ft_strlen(input) - 1;
	while (ptr >= input && *ptr == ' ')
		ptr--; 
	if (ptr >= input && *ptr == '|')
		return (1); // Erreur: commande finit avec '|'
	return (0);
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
