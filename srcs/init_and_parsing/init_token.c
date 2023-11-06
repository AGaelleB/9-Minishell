/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 14:05:00 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/04 12:29:01 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*new_token(t_e_token_type e_type, char *split_value)
{
	t_token	*token;

	token = NULL;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = e_type;
	token->split_value = ft_strdup(split_value); // Remember to free this later!
	token->next = NULL;
	token->prev = NULL;
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
		new_tok->prev = *tail;
		(*tail)->next = new_tok;
		*tail = new_tok;
	}
}

void	init_tokenizer(t_tokenizer *tz, char *input)
{
	tz->i = 0;
	tz->head = NULL;
	tz->curr = NULL;
	tz->token = NULL; //NEW init ici
	tz->state = TYPE_CMD;
	tz->cmd_processed = false;
	tz->flag_single_quote = false;
	tz->flag_double_quote = false;
	tz->delimiters[0] = " ";
	tz->delimiters[1] = ">>";
	tz->delimiters[2] = "<<";
	tz->delimiters[3] = "<";
	tz->delimiters[4] = ">";
	tz->delimiters[5] = NULL;
	tz->words = split_string_token(input, tz->delimiters);
}

t_token	*create_token(t_tokenizer *tz, t_env *env)
{
	tz->token = NULL;
	if (tz->state == TYPE_F_OUT || tz->state == TYPE_F_IN)
	{
		tz->token = handle_arg_token(tz);
		return (tz->token);
	}
	if (tz->words[tz->i] != NULL && !tz->token)
		tz->token = handle_cmd_token(tz, env);
	if (contains_single_quote(tz->words[tz->i]) && !tz->token)
		tz->token = handle_single_quote_token(tz);
	else if (contains_double_quote(tz->words[tz->i]) && !tz->token)
		tz->token = handle_double_quote_token(tz);
	if (!tz->flag_single_quote && !tz->flag_double_quote && !tz->token)
		tz->token = handle_redir_tokens(tz);
	if (!tz->token)
		tz->token = handle_arg_token(tz);
	return (tz->token);
}

// t_token	*tokenize_input(char *input, t_env *env)
// {
// 	t_tokenizer	*tz;

// 	tz = malloc(sizeof(t_tokenizer));//NEW
// 	init_tokenizer(tz, input);
// 	while (tz->words[tz->i])
// 	{
// 		if (!is_empty_or_space(tz->words[tz->i]))
// 		{
// 			tz->token = malloc(sizeof(t_token) * 1000); //NEW init ici
// 			tz->token = create_token(tz, env);
// 			add_token_to_list(&tz->head, &tz->curr, tz->token);
// 			// free(tz.token); //on a plus rien ca tourne dans le vide
// 		}
// 		tz->i++;
// 	}
// 	ft_free_tab(tz->words);
// 	return (tz->head);
// }

t_token	*tokenize_input(char *input, t_env *env)
{
	t_tokenizer	tz;

	init_tokenizer(&tz, input);
	while (tz.words[tz.i])
	{
		if (!is_empty_or_space(tz.words[tz.i]))
		{
			tz.token = create_token(&tz, env);
			add_token_to_list(&tz.head, &tz.curr, tz.token);
			// free(tz.token);
		}
		tz.i++;
	}
	ft_free_tab(tz.words);
	return (tz.head);
}
