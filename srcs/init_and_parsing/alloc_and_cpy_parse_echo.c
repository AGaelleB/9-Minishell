/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/09 17:01:27 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quotes_echo(char *str, int *i, bool *double_quote, bool *in_quote)
{
	if (!*in_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		(*i) += 2;
	else if (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
		(*i) += 2;
	if (!*double_quote && str[*i] == '\'')
		*in_quote = !*in_quote;
	else if (!*in_quote && str[*i] == '\"')
		*double_quote = !*double_quote;
	if (!*in_quote && str[*i] == '\"')
		(*i)++;
	else if (!*double_quote && str[*i] == '\'')
		(*i)++;
}


// char	*ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
// {
// 	char	*arg;
// 	bool	double_quote;
// 	bool	single_quote;

// 	double_quote = false;
// 	single_quote = false;
// 	arg = malloc(sizeof(char *) * SIZE); //arg = malloc(sizeof(char*) * SIZE);
// 	if (!arg)
// 		exit(EXIT_FAILURE);
// 	*arg_idx = 0;
// 	while (input[*i] && (double_quote || single_quote || input[*i] != ' '))
// 	{
// 		handle_quotes_echo(input, i, &double_quote, &single_quote);
// 		if (is_redirection(input[*i]) && !double_quote && !single_quote)
// 			ft_skip_redirection_and_file(input, i);
// 		else if (input[*i] == '$' && !single_quote)
// 			handle_arg_value(env, input, i, arg, arg_idx);
// 		else
// 			arg[(*arg_idx)++] = input[(*i)++];
// 	}
// 	arg[*arg_idx] = '\0';
// 	skip_spaces_echo(input, i);
// 	return (arg);
// }

t_arg_handler init_handler(t_env *env, char *input, int *i, int *arg_idx)
{
	t_arg_handler handler;

	handler.env = env;
	handler.input = input;
	handler.i = i;
	handler.arg_idx = arg_idx;
	return(handler);
}

char *ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
    t_arg_handler	handler;
    bool			double_quote = false;
    bool			single_quote = false;

    handler = init_handler(env, input, i, arg_idx);
    handler.arg = malloc(sizeof(char) * SIZE);  // Attention à la taille allouée.
    if (!handler.arg)
        exit(EXIT_FAILURE);

    *handler.arg_idx = 0;
    while (handler.input[*handler.i])
    {
        if (handler.input[*handler.i] == ' ' && !double_quote && !single_quote) {
            break;  // Arrêt si espace et pas dans les guillemets.
        }

        handle_quotes_echo(handler.input, handler.i, &double_quote, &single_quote);
        if (is_redirection(handler.input[*handler.i]) && !double_quote && !single_quote) {
            ft_skip_redirection_and_file(handler.input, handler.i);
        } else if (handler.input[*handler.i] == '$' && !single_quote) {
            handle_arg_value(&handler);
        } else {
            handler.arg[(*handler.arg_idx)++] = handler.input[*handler.i];
            (*handler.i)++;
        }
    }

    handler.arg[*handler.arg_idx] = '\0';
    return (handler.arg);
}


/* char *ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
	t_arg_handler	handler;
	bool			double_quote;
	bool			single_quote;

	double_quote = false;
	single_quote = false;
	handler.arg = malloc(sizeof(char) * SIZE);
	if (!handler.arg)
		exit(EXIT_FAILURE);
	handler = init_handler(env, input, i, arg_idx);
	*handler.arg_idx = 0;
	while (handler.input[*handler.i] && (double_quote || single_quote || handler.input[*handler.i] != ' '))
	{
		handle_quotes_echo(handler.input, handler.i, &double_quote, &single_quote);
		if (is_redirection(handler.input[*handler.i]) && !double_quote && !single_quote)
			ft_skip_redirection_and_file(handler.input, handler.i);
		else if (handler.input[*handler.i] == '$' && !single_quote)
			handle_arg_value(&handler);  // Passez l'adresse de handler.
		else
			handler.arg[(*handler.arg_idx)++] = handler.input[(*handler.i)++];
	}
	handler.arg[*handler.arg_idx] = '\0';
	skip_spaces_echo(handler.input, handler.i);
	return (handler.arg);
} */