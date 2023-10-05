/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 14:48:27 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_args(char *input)
{
	int		count;
	bool	in_quote;
	bool	was_space; // pour compter le premier mot même s'il n'y a pas d'espace devant
	
	count = 0;
	in_quote = false;
	was_space = true;
	while (*input)
	{
		if (*input == '\'')
			in_quote = !in_quote;
		if (!in_quote && *input == ' ')
			was_space = true;
		else if (was_space)
		{
			count++;
			was_space = false;
		}
		input++;
	}
	return (count);
}

int	is_redir_at_beginning(char *input, int i)
{
	while (input[i] == ' ')
		i++;

	if ((input[i] == '>') || (input[i] == '<')
		|| (input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
	{
		if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
			i += 2;
		else if (input[i] == '>' || input[i] == '<')
			i++;
		while (input[i] == ' ')
			i++;
		while (input[i] != ' ')
			i++;
		return (is_redir_at_beginning(input, i));
	}
	return (i);
}

char	**parse_input_quote(char *input)
{
	bool	in_quote;
	int		arg_count;
	int		arg_idx;
	int		idx;
	char	**args;
	char	*arg;
	int		i;

	i = 0;
	i = is_redir_at_beginning(input, i);
	arg_count = count_args(input);
	args = malloc((arg_count + 1) * sizeof(char *));
	in_quote = false;
	idx = 0;
	if (!args)
		return (NULL);
	while (input[i])
	{
		arg = malloc(ft_strlen(input) + 1);
		if (!arg)
			return (NULL);
		arg_idx = 0;
		while (input[i] && (in_quote || (input[i] != ' ' && input[i] != '>' && input[i] != '<')))
		{
			if (input[i] == '\'')
				in_quote = !in_quote;
			else
				arg[arg_idx++] = input[i];
			i++;
		}
		if ((!in_quote) && (input[i] == '>' || input[i] == '<'))
			break;
		arg[arg_idx] = '\0';
		if (arg_idx > 0)
			args[idx++] = arg;
		else
			free(arg);
		while (input[i] == ' ')
			i++;
	}
	args[idx] = NULL;
	// free(arg);
	return (args);
}
