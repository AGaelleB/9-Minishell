/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_valid_identifier_export.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 16:48:37 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/15 16:26:03 by bfresque         ###   ########.fr       */
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

int	check_before_equal(char *s)
{
	int		i;

	i = 0;
	while (s[i] == ' ')
		i++;
	if (s[i] == '\0')
		return (print_error_export(s, 1));
	if (ft_isdigit(s[0]) || check_valid_identifier_bis_bis(s[0]))
		return (print_error_export(s, 1));
	if (s[i] == '$')
		return (1);
	i = 1;
	while (s[i] && (s[i] != '='))
	{
		// if (s[i] == '\0')
		// 	break ;
		if (check_valid_identifier(s[i]))
			return (print_error_export(s, 1));
		i++;
	}
	return (0);
}

int	check_after_equal(char *str)
{
	int		i;
	bool	single_quote;
	bool	double_quote;

	i = 0;
	single_quote = false;
	double_quote = false;
	while ((str[i] != '=') && str[i])
		i++;
	if (str[i] == '=')
		i++;
	while (str[i])
	{
		if (str[i] == '\'')
			single_quote = !single_quote;
		else if (str[i] == '\"')
			double_quote = !double_quote;
		while (str[i] == ' ' && (double_quote || single_quote))
			i++;
		if ((!double_quote && !single_quote) && (check_valid_identifier_bis(str[i]) == 1))
			return (1);
		i++;
	}
	return (0);
}
