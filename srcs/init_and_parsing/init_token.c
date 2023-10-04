/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 14:05:00 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/04 14:46:59 by abonnefo         ###   ########.fr       */
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

void add_token_to_list(t_token **head, t_token **tail, t_token *new_tok) 
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

bool contains_single_quote(char *str)
{
	while (*str)
	{
		if (*str == '\'')
			return true;
		str++;
	}
	return false;
}

t_token *tokenize_input(char *input, char **envp)
{
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	char		**words;
	int			i;
	int			state;
	bool		flag_single_quote;
	char		*delimiters[6];

	i = 0;
	head = NULL;
	curr = NULL;
	state = TYPE_CMD;
	flag_single_quote = false;
	delimiters[0] = " ";
	delimiters[1] = "<";
	delimiters[2] = ">>";
	delimiters[3] = "<<";
	delimiters[4] = ">";
	delimiters[5] = NULL;
	words = split_string_token(input, delimiters);

	while (words[i])
	{
		printf("words[%d]: %s\n", i, words[i]);
		i++;
	}
	i = 0;

	while (words[i])
	{
		// flag_single_quote = !flag_single_quote;
		if (is_empty_or_space(words[i]))
		{
			i++;
			continue;
		}
		token = NULL;
		char *path = NULL;
		// (void)envp; 

		if (words[i] != NULL)
			path = ft_check_paths(envp, words[i]);
		// printf("		words[%d] = %s\n", i, words[i]);
		// printf("		path = %s\n", path);
		if (path != NULL) //  state == TYPE_CMD)
		{
			// printf("PAS NULL\n");
			token = new_token(TYPE_CMD, words[i]);
			state = TYPE_ARG;
			free(path);
		}

		
		// if (state == TYPE_CMD)
		// {
		// 	token = new_token(TYPE_CMD, words[i]);
		// 	state = TYPE_ARG;
		// }
		else if (contains_single_quote(words[i]))
		{
			flag_single_quote = !flag_single_quote;
			token = new_token(TYPE_ARG, words[i]);
			state = TYPE_ARG;
		}
		else if ((!flag_single_quote) && (ft_strcmp_minishell(words[i], ">") == 0))
		{
			token = new_token(TYPE_REDIR_OUT, words[i]);
			state = TYPE_F_OUT;
		}
		else if ((!flag_single_quote) && (ft_strcmp_minishell(words[i], "<") == 0))
		{
			token = new_token(TYPE_REDIR_IN, words[i]);
			state = TYPE_F_IN;
		}
		else if ((!flag_single_quote) && (ft_strcmp_minishell(words[i], ">>") == 0))
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
		add_token_to_list(&head, &curr, token);
		i++;
		printf("%stoken->type = %d%s\n", MAGENTA, token->type, RESET); ////////
	}
	ft_free_tab(words);
	return (head);
}


// t_token *tokenize_input(char *input, char **envp)
// {
// 	// printf("*** tokenize_input ***\n");
// 	t_token		*head;
// 	t_token		*curr;
// 	t_token		*token;
// 	char		**words;
// 	int			i;
// 	int			state;
// 	bool		flag_single_quote;
// 	char		*delimiters[6];
	
// 	i = 0;
// 	head = NULL;
// 	curr = NULL;
// 	state = TYPE_CMD; //
// 	// state = TYPE_ARG;
// 	flag_single_quote = false;
// 	delimiters[0] = " ";
// 	delimiters[1] = "<";
// 	delimiters[2] = ">>";
// 	delimiters[3] = "<<";
// 	delimiters[4] = ">";
// 	delimiters[5] = NULL;
// 	words = split_string_token(input, delimiters);

// 	// while (words[i])
// 	// {
// 	// 	printf("words[%d]: %s\n", i, words[i]);
// 	// 	i++;
// 	// }
// 	// i = 0;
	

// 	char *path;
	

// 	while (words[i])
// 	{
// 		if (is_empty_or_space(words[i]))
// 		{
// 			i++;
// 			continue;
// 		}
// 		token = NULL;
// 		path = NULL;
// 		(void)envp; 

// 		if (words[i] != NULL)
// 			path = ft_check_paths(envp, words[i]);
// 		// printf("		words[%d] = %s\n", i, words[i]);
// 		// printf("		path = %s\n", path);
// 		if (path != NULL) //  state == TYPE_CMD)
// 		{
// 			// printf("PAS NULL\n");
// 			token = new_token(TYPE_CMD, words[i]);
// 			state = TYPE_ARG;
// 			free(path);
// 		}
// 		//  if (state == TYPE_CMD)
// 		// {
// 		// 	printf("NULL\n");
// 		// 	token = new_token(TYPE_CMD, words[i]);
// 		// 	state = TYPE_ARG;
// 		// }
// 		else if (contains_single_quote(words[i]))
// 		{
// 			flag_single_quote = !flag_single_quote;
// 			token = new_token(TYPE_ARG, words[i]);
// 			state = TYPE_ARG;
// 		}
// 		else if ((!flag_single_quote) && (ft_strcmp_minishell(words[i], ">") == 0))
// 		{
// 			token = new_token(TYPE_REDIR_OUT, words[i]);
// 			state = TYPE_F_OUT;
// 		}
// 		else if ((!flag_single_quote) && (ft_strcmp_minishell(words[i], "<") == 0))
// 		{
// 			token = new_token(TYPE_REDIR_IN, words[i]);
// 			state = TYPE_F_IN;
// 		}
// 		else if ((!flag_single_quote) && (ft_strcmp_minishell(words[i], ">>") == 0))
// 		{
// 			token = new_token(TYPE_REDIR_APPEND, words[i]);
// 			state = TYPE_F_OUT;
// 		}
// 		else if (ft_strcmp_minishell(words[i], "<<") == 0) // utiliser le bool ?? 
// 		{
// 			token = new_token(TYPE_HEREDOC, words[i]);
// 			state = TYPE_EOF;
// 		}
// 		else if (state == TYPE_F_OUT) // > // >>
// 		{
// 			token = new_token(TYPE_F_OUT, words[i]);
// 			state = TYPE_ARG;
// 		}
// 		else if (state == TYPE_F_IN) // <
// 		{
// 			token = new_token(TYPE_F_IN, words[i]);
// 			state = TYPE_ARG;
// 		}
// 		else if (state == TYPE_EOF) // <<
// 		{
// 			token = new_token(TYPE_EOF, words[i]);
// 			state = TYPE_ARG;
// 		}
// 		else
// 		{
// 			token = new_token(TYPE_ARG, words[i]);
// 			state = TYPE_ARG; //
// 		}
// 		add_token_to_list(&head, &curr, token);
// 		i++;
// 		printf("%stoken->type = %d%s\n\n", MAGENTA, token->type, RESET); ////////
		
// 	}
// 	ft_free_tab(words);
// 	return (head);
// }
