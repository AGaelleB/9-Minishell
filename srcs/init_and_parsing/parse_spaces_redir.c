/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_spaces_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:15:47 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/01 16:08:47 by abonnefo         ###   ########.fr       */
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
	return (new_len + 1);
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

char	*add_sp_ard_redir(char *input, t_env *env)
{
	char	*new_input;
	int		new_len;

	(void)env;
	new_len = calculate_new_len(input);
	new_input = malloc(sizeof(char) * SIZE);
	if (!new_input)
		return (NULL);
	ft_memset(new_input, '\0', new_len + 1);
	return (copy_with_spaces(input, new_input));
}
