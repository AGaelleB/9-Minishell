/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_around_equal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/15 17:32:40 by bfresque          #+#    #+#             */
/*   Updated: 2023/12/04 15:50:07 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if ((!double_quote && !single_quote)
			&& (check_valid_identifier_bis(str[i]) == 1))
			return (1);
		i++;
	}
	return (0);
}
