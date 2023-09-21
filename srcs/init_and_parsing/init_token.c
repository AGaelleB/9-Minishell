/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 14:05:00 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/21 09:45:34 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// Fonction utilitaire pour vérifier si une chaîne est vide ou composée uniquement d'espaces

t_token *new_token(t_e_token_type e_type, char *split_value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);

	token->type = e_type;
	token->split_value = ft_strdup(split_value);  // Remember to free this later!
	token->next = (NULL);
	// printf("value : %s, type: %d\n", token->split_value, token->type);


	return (token);
}

t_token *tokenize_input(char *input)
{
	char		**words;
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	int			i;
	int			state = TYPE_CMD;  // Start with the command state

	words = split_string(input, ' ');
	head = NULL;
	curr = NULL;
	i = 0;

	while (words[i])
	{
		if (is_empty_or_space(words[i]))
		{
			i++;
			continue;
		}

		token = NULL;

		if (state == TYPE_CMD)
		{
			token = new_token(TYPE_CMD, words[i]);
			state = TYPE_ARG;
		}
		else if (ft_strcmp_minishell(words[i], ">") == 0)
		{
			token = new_token(TYPE_REDIR_OUT, words[i]);
			state = TYPE_F_OUT;
		}
		else if (ft_strcmp_minishell(words[i], "<") == 0)
		{
			token = new_token(TYPE_REDIR_IN, words[i]);
			state = TYPE_F_IN;  // expect an output file next
		}
		else if (ft_strcmp_minishell(words[i], ">>") == 0)
		{
			token = new_token(TYPE_REDIR_APPEND, words[i]);
			state = TYPE_F_OUT;  // expect an output file next
		}
		else if (ft_strcmp_minishell(words[i], "<<") == 0)
		{
			token = new_token(TYPE_DELIMITATOR, words[i]);
			state = TYPE_F_IN;  // expect an output file next
		}
		else if (state == TYPE_F_OUT)
		{
			token = new_token(TYPE_F_OUT, words[i]);
			state = TYPE_ARG;  // switch back to argument state
		}
		else if (state == TYPE_F_IN)
		{
			token = new_token(TYPE_F_IN, words[i]);
			state = TYPE_ARG;  // switch back to argument state
		}
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
	return head;
}
