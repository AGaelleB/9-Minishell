/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 14:05:00 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/06 15:40:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*new_token(t_e_token_type e_type, char *split_value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = e_type;
	token->split_value = ft_strdup(split_value); // Remember to free this later!
	token->next = (NULL);
	// printf("value : %s | type: %d\n", token->split_value, token->type);
	return (token);
}

void	add_token_to_list(t_token **head, t_token **tail, t_token *new_tok)
{
	if (!*head)
	{
		*head = new_tok;
		*tail = *head;
	}
	else
	{
		(*tail)->next = new_tok;
		*tail = new_tok;
	}
}

void	init_tokenizer(t_tokenizer *tz, char *input)
{
	tz->i = 0;
	tz->head = NULL;
	tz->curr = NULL;
	tz->state = TYPE_CMD; // -1
	tz->flag_single_quote = false;
	tz->delimiters[0] = " ";
	tz->delimiters[1] = ">>";
	tz->delimiters[2] = "<<";
	tz->delimiters[3] = "<";
	tz->delimiters[4] = ">";
	tz->delimiters[5] = NULL;
	tz->words = split_string_token(input, tz->delimiters);
}

t_token	*create_token(t_tokenizer *tz, char **envp)
{
	tz->token = NULL;
	if (tz->words[tz->i] != NULL && !tz->token)
		tz->token = handle_cmd_token(tz, envp);
	if (contains_single_quote(tz->words[tz->i]) && !tz->token)
		tz->token = handle_quote_token(tz);
	if (!tz->flag_single_quote && !tz->token)
		tz->token = handle_redir_tokens(tz);
	if (!tz->token)
		tz->token = handle_arg_token(tz);
	// printf("%stz->flag_single_quote = %d%s\n", YELLOW, tz->flag_single_quote, RESET);
	return (tz->token);
}

t_token	*tokenize_input(char *input, char **envp)
{
	t_tokenizer	tz;

	init_tokenizer(&tz, input);
	while (tz.words[tz.i])
	{
		if (!is_empty_or_space(tz.words[tz.i]))
		{
			tz.token = create_token(&tz, envp);
			add_token_to_list(&tz.head, &tz.curr, tz.token);
		}
		tz.i++;
	}
	ft_free_tab(tz.words);
	return (tz.head);
}
