/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 15:08:08 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/11 15:10:00 by abonnefo         ###   ########.fr       */
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

int check_syntax_errors(char *input)
{
	char	*ptr;
	int		pipe_found;
	
	ptr = input;
	pipe_found = 0;
	while (*ptr)
	{
		if (*ptr == '|')
		{
			if (pipe_found)
				return (2); // Erreur: "||" trouvé
			pipe_found = 1; 
		}
		else if (*ptr != ' ')
		{
			pipe_found = 0;
		}
		ptr++;
	}
	ptr = input + strlen(input) - 1;
	while (ptr >= input && *ptr == ' ')
		ptr--; 
	if (ptr >= input && *ptr == '|')
		return (1); // Erreur: commande finit avec '|'
	return (0);
}

int pipe_syntax_errors(char *input)
{
	int	syntax_error_code;

	syntax_error_code = check_syntax_errors(input);
	if (syntax_error_code == 1)
	{
		ft_putstr_fd("minishell: syntax error command required after '|'\n", 2);
		free(input);
		return (-1);
	}
	else if (syntax_error_code == 2)
	{
		ft_putstr_fd("minishell: syntax error command required after '||'\n", 2);
		free(input);
		return (-1);
	}
	return (0);
}
