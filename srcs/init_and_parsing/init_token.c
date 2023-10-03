/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 14:05:00 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/03 10:00:38 by abonnefo         ###   ########.fr       */
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
	int			state;
	bool		flag_single_quote;

	i = 0;
	head = NULL;
	curr = NULL;
	state = TYPE_CMD;
	words = split_string(input, ' ');
	flag_single_quote = false;
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
		else if (*words[i] == '\'')
		{
			flag_single_quote = !flag_single_quote;
			token = new_token(TYPE_ARG, words[i]);
			state = TYPE_ARG;
		}
		else if ((flag_single_quote == 0) && (ft_strcmp_minishell(words[i], ">") == 0))
		{
			token = new_token(TYPE_REDIR_OUT, words[i]);
			state = TYPE_F_OUT;
		}
		else if ((flag_single_quote == 0) && (ft_strcmp_minishell(words[i], "<") == 0))
		{
			token = new_token(TYPE_REDIR_IN, words[i]);
			state = TYPE_F_IN;
		}
		else if ((flag_single_quote == 0) && (ft_strcmp_minishell(words[i], ">>") == 0))
		{
			token = new_token(TYPE_REDIR_APPEND, words[i]);
			state = TYPE_F_OUT;
		}
		else if (ft_strcmp_minishell(words[i], "<<") == 0) // utiliser le bool ?? 
		{
			token = new_token(TYPE_HEREDOC, words[i]);
			state = TYPE_EOF;
		}
		else if (state == TYPE_F_OUT) // > // >>
		{
			token = new_token(TYPE_F_OUT, words[i]);
			state = TYPE_ARG;
		}
		else if (state == TYPE_F_IN) // <
		{
			token = new_token(TYPE_F_IN, words[i]);
			state = TYPE_ARG;
		}
		else if (state == TYPE_EOF) // <<
		{
			token = new_token(TYPE_EOF, words[i]);
			state = TYPE_ARG;
		}
		else
		{
			token = new_token(TYPE_ARG, words[i]);
			state = TYPE_ARG; //
		}
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
	return (head);
}
