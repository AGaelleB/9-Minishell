/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_string_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:33:04 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/06 17:52:42 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	reset_values(t_split_token *token, int j_value)
{
	token->j = j_value;
	token->start = j_value;
}

static int	assign_and_check(t_split_token *token, char **tokens, char *str)
{
	tokens[token->i] = allocate_and_cpy(str + token->start, token->len_to_copy);
	if (!tokens[token->i])
		return (0);
	token->i++;
	return (1);
}

static char	**create_tokens_sub(char **tokens, char *str, t_split_token *token)
{
	if (token->delim_len > 0)
		token->len_to_copy = token->j - token->start;
	else
		token->len_to_copy = token->j - token->start + 1;
	if (!assign_and_check(token, tokens, str))
		return (free_token_split(tokens));
	if (token->delim_len > 0)
	{
		token->start = token->j;
		token->len_to_copy = token->delim_len;
		if (!assign_and_check(token, tokens, str))
			return (free_token_split(tokens));
		reset_values(token, token->j + token->delim_len);
	}
	else
		reset_values(token, ++(token->j));
	return (tokens);
}

char	**create_tokens(char *str, char **delimiters, char **tokens)
{
	t_split_token	token;

	ft_memset(&token, 0, sizeof(t_split_token));
	while (str[token.j])
	{
		if (is_delimiter(&str[token.j], delimiters, &(token.delim_len))
			|| str[token.j + 1] == '\0')
		{
			tokens = create_tokens_sub(tokens, str, &token);
			if (!tokens)
				return (NULL);
		}
		else
			token.j++;
	}
	tokens[token.i] = NULL;
	return (tokens);
}

char	**split_string_token(char *str, char **delimiters)
{
	char	**tokens;
	int		token_count;

	token_count = count_split_tokens_str(str, delimiters);
	tokens = allocate_tokens(token_count);
	if (!tokens)
		return (NULL);
	return (create_tokens(str, delimiters, tokens));
}
