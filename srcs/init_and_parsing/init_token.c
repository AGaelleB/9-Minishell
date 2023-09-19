/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 14:05:00 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/18 16:14:40 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token *new_token(t_e_token_type e_type, char *split_value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);

	token->type = e_type;
	token->split_value = ft_strdup(split_value);  // Remember to free this later!
	token->next = (NULL);
	printf("value : %s, type: %d\n", token->split_value, token->type);


	return (token);
}

t_token *tokenize_input(char *input)
{
	char		**words;
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	int			i;

	words = split_string(input, ' ');
	head = NULL;
	curr = NULL;
	i = 0;
	while (words[i])
	{
		token = NULL;
		if (ft_strcmp_minishell(words[i], "|") == 0)
			token = new_token(TYPE_SEPARATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], ">") == 0)
			token = new_token(TYPE_REDIR_OUT, words[i]);
		else if (ft_strcmp_minishell(words[i], "<") == 0)
			token = new_token(TYPE_REDIR_IN, words[i]);
		else if (ft_strcmp_minishell(words[i], ">>") == 0)
			token = new_token(TYPE_DELIMITATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], "<<") == 0)
			token = new_token(TYPE_REDIR_APPEND, words[i]);
		else if (i == 0 ||
				(i > 0 && (ft_strcmp_minishell(words[i - 1], "|") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<") == 0 ||
							ft_strcmp_minishell(words[i - 1], ">>") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<<") == 0)))
			token = new_token(TYPE_CMD, words[i]);
		else if(i > 0 && ft_strcmp_minishell(words[i - 1], ">") == 0)
			token = new_token(TYPE_F_OUT, words[i]);
		else
			token = new_token(TYPE_ARG, words[i]);
		if (!head)
		{
			head = token;
			curr = head;
		}
		else
		{
			curr->next = token;
			curr = token;
		}
		i++;
	}
	ft_free_tab(words);
	// ft_free_tokens(head);
	return (head);
}
