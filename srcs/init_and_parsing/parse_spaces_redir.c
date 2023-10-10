/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_spaces_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:15:47 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/10 15:35:31 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	calculate_new_len(char *input)
{
	int	i;
	int	new_len;

	i = 0;
	new_len = ft_strlen(input);
	while (input[i])
	{
		if (input[i] == '>' || input[i] == '<')
			new_len += 2;
		i++;
	}
	return (new_len);
}

char	*copy_with_spaces(char *input, char *new_input)
{
	int		i;
	int		j;
	bool	quote;

	i = 0;
	j = 0;
	quote = false;
	while (input[i])
	{
		if (input[i] == '\'' || input[i] == '\"')
			quote = !quote;
		if ((!quote) && (input[i] == '>' || input[i] == '<'))
		{
			new_input[j++] = ' ';
			new_input[j++] = input[i];
			if (input[i + 1] == input[i])
				new_input[j++] = input[++i];
			new_input[j++] = ' ';
		}
		else
			new_input[j++] = input[i];
		i++;
	}
	new_input[j] = '\0';
	return (new_input);
}

char	*add_spaces_around_redirections(char *input)
{
	char	*new_input;
	int		new_len;

	new_len = calculate_new_len(input);
	new_input = malloc(sizeof(char) * (new_len + 1));
	if (!new_input)
	{
		perror("malloc");
		return (NULL);
	}
	return (copy_with_spaces(input, new_input));
}
