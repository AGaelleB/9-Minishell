/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command_on_pipe.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 14:59:02 by bfresque          #+#    #+#             */
/*   Updated: 2023/12/02 11:39:35 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strndup(char *s, size_t n)
{
	char	*new_str;
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	if (len > n)
		len = n;
	new_str = malloc(sizeof(char *) * (len + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_str[i] = s[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int	count_commands(char *input)
{
	int	count;
	int	inside_quotes;
	int	i;

	count = 1;
	inside_quotes = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '"')
			inside_quotes = !inside_quotes;
		if (input[i] == '|' && !inside_quotes)
			count++;
		i++;
	}
	return (count);
}

char	**fill_commands(char *input, char **command, int count)
{
	char	*temp;
	char	*start;
	int		inside_quotes;

	inside_quotes = 0;
	start = input;
	while (*input)
	{
		if (*input == '"')
			inside_quotes = !inside_quotes;
		if (*input == '|' && !inside_quotes)
		{
			temp = ft_strndup(start, input - start);
			command[count++] = ft_strtrim(temp, " ");
			free(temp);
			start = input + 1;
		}
		input++;
	}
	temp = ft_strndup(start, input - start);
	command[count++] = ft_strtrim(temp, " ");
	free(temp);
	command[count] = NULL;
	return (command);
}

char	**split_command_on_pipe(char *input)
{
	char	**command;
	int		count;

	count = count_commands(input);
	command = malloc(sizeof(char *) * (count + 1));
	if (!command)
		return (NULL);
	return (fill_commands(input, command, 0));
}
