/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 15:21:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/06 14:26:26 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_token	*handle_cmd_token(t_tokenizer *tz, t_env *env)
{
	char	*path;

	path = ft_check_paths_token(env, tz->words[tz->i]);
	if (path)
	{
		if (!tz->cmd_processed)
		{
			tz->token = new_token(TYPE_CMD, tz->words[tz->i]);
			tz->cmd_processed = true;
		}
		else
			tz->token = new_token(TYPE_ARG, tz->words[tz->i]);
		tz->state = TYPE_ARG;
		free(path);
	}
	return (tz->token);
}

t_token	*handle_redir_tokens(t_tokenizer *tz)
{
	if (ft_strcmp_minishell(tz->words[tz->i], "<<") == 0)
	{
		tz->token = new_token(TYPE_HEREDOC, tz->words[tz->i]);
		tz->state = TYPE_EOF;
	}
	else if (ft_strcmp_minishell(tz->words[tz->i], ">>") == 0)
	{
		tz->token = new_token(TYPE_REDIR_APPEND, tz->words[tz->i]);
		tz->state = TYPE_F_OUT;
	}
	else if (ft_strcmp_minishell(tz->words[tz->i], ">") == 0)
	{
		tz->token = new_token(TYPE_REDIR_OUT, tz->words[tz->i]);
		tz->state = TYPE_F_OUT;
	}
	else if (ft_strcmp_minishell(tz->words[tz->i], "<") == 0)
	{
		tz->token = new_token(TYPE_REDIR_IN, tz->words[tz->i]);
		tz->state = TYPE_F_IN;
	}
	return (tz->token);
}

t_token	*handle_arg_token(t_tokenizer *tz)
{
	if (tz->state == TYPE_F_OUT || tz->state == TYPE_F_IN
		|| tz->state == TYPE_EOF)
	{
		tz->token = new_token(tz->state, tz->words[tz->i]);
		tz->state = TYPE_ARG;
	}
	else
		tz->token = new_token(TYPE_ARG, tz->words[tz->i]);
	return (tz->token);
}

t_token	*handle_single_quote_token(t_tokenizer *tz)
{
	tz->flag_single_quote = !tz->flag_single_quote;
	tz->token = new_token(TYPE_ARG, tz->words[tz->i]);
	tz->state = TYPE_ARG;
	return (tz->token);
}

t_token	*handle_double_quote_token(t_tokenizer *tz)
{
	tz->flag_double_quote = !tz->flag_double_quote;
	tz->token = new_token(TYPE_ARG, tz->words[tz->i]);
	tz->state = TYPE_ARG;
	return (tz->token);
}
