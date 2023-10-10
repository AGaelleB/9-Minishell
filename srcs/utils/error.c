/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 15:21:13 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/10 15:59:06 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_print_error(char *str)
{
	int	len;

	if (!ft_strncmp(str, "cd", ft_strlen(str)))
		return ;
	len = ft_strlen(str);
	write(2, "command not found :", 19);
	write(2, str, len);
	write(2, "\n", 1);
}

void	exit_with_error(char *message, pid_t *child_pids)
{
	perror(message);
	free(child_pids);
	exit(1);
}

int	check_valid_identifier(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == '\'' || c == '\"' || c == ',' || c == '.'
		|| c == ':' || c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~' || c == ';'
		|| c == '=' || c == '-' || c == '?' || c == '&' || c == '*') //|| c == ' '
	{
		return (1);
	}
	else
		return (0);
}

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