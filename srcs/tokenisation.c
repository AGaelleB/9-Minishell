/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 14:05:06 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/07 14:19:35 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**split_string(const char *str, char delimiter)
{
	int i;
	int j;
	int k;
	int token_count;
	char **tokens;
	int start;
	int token_size;

	token_count = 1;
	i = 0;
	while(str[i])
	{
		if (str[i] == delimiter)
			token_count++;
		i++;
	}

	tokens = malloc((token_count + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("malloc");
		return NULL;
	}

	start = 0;
	i = 0;
	j = 0;

	while(str[j])
	{
		if (str[j] == delimiter || str[j + 1] == '\0')
		{
			if (str[j] == delimiter)
				token_size = j - start;
			else
				token_size = j - start + 1;
			tokens[i] = malloc(token_size + 1);
			if (!tokens[i])
			{
				perror("malloc");
				k = 0;
				while(k < i)
				{
					free(tokens[k]);
					k++;
				}
				free(tokens);
				return (NULL);
			}
			strncpy(tokens[i], &str[start], token_size);
			tokens[i][token_size] = '\0';
			start = j + 1;
			i++;
		}
		j++;
	}
	tokens[i] = NULL;
	return (tokens);
}

t_command	*get_command_and_separator(char *input)
{
	// t_command	*head; //
	t_command	*current;
	t_command	*new_cmd;
	char		**tokens;
	int			i;

	// head = NULL; // 
	current = NULL;
	tokens = split_string(input, '|');
	i = 0;
	while(tokens[i])
	{
		new_cmd = malloc(sizeof(t_command));
		new_cmd->command = ft_strdup(tokens[i]);
		// if (tokens[i + 1])
		// 	new_cmd->separator = '|';
		// else
		// 	new_cmd->separator = '\0';
		// new_cmd->next = NULL;
		if (!current)
		{
			current = new_cmd;
			// current = head; // 
		}
		else
		{
			current->next = new_cmd;
			current = new_cmd;
		}
		i++;
	}
	return (current);
	
// //////////////////////////////////////////////////////////////////////////////////////////
// 	current = head;  // Réinitialisation à la tête de la liste pour l'impression

// 	while(current)  // Parcourir jusqu'à ce que current soit NULL
// 	{
// 		printf("cmd = %s\n", current->command);
// 		current = current->next;
// 	}
// //////////////////////////////////////////////////////////////////////////////////////////
}
