/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_valid_identifier_export.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:48:37 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/04 15:47:00 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_char_valid(char c)
{
	if (ft_isalnum(c) || c == '_' || c == '='
		|| c == '\'' || c == '\"' || c == '.'
		|| c == '$' || c == ':' || c == '-')
	{
		return (0);
	}
	else
		return (1);
}

int	print_error_export(char *s, int ok)
{
	int	i;

	i = 0;
	if (ok)
	{
		ft_putstr_fd("minishell: export: `", 2);
		while (s[i] && s[i] != '=')
			ft_putchar_fd(s[i++], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (g_exit_status = 1);
	}
	return (0);
}

int	check_valid_identifier(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == '\'' || c == '\"' || c == ' ' || c == ',' || c == '.'
		|| c == ':' || c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~'
		|| c == '=' || c == '-' || c == '?' || c == '&' || c == '*')
	{
		return (1);
	}
	else
		return (0);
}

int	check_valid_identifier_bis(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == ' ' || c == ',' || c == '.'
		|| c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~'
		|| c == '=' || c == '-' || c == '?' || c == '&' || c == '*')
	{
		return (1);
	}
	else
		return (0);
}

int	check_valid_identifier_bis_bis(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '[' || c == ']'
		|| c == ' ' || c == ',' || c == '.'
		|| c == '/' || c == '{' || c == '}' || c == '+'
		|| c == '^' || c == '%' || c == '#' || c == '@' || c == '!'
		|| c == '~' || c == ':' //ICI
		|| c == '=' || c == '-' || c == '?' || c == '&' || c == '*')
	{
		return (1);
	}
	else
		return (0);
}
