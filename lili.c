t_token *tokenize_input(char *input)
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
		if (state == TYPE_CMD)
		{
			token = new_token(TYPE_CMD, words[i]);
			state = TYPE_ARG;
		}
		else if (*words[i] == '\'')
		{
			// printf("%sflag_single_quote %d%s\n", YELLOW, flag_single_quote, RESET);
			flag_single_quote = !flag_single_quote;
			// printf("%sflag_single_quote %d%s\n", GREEN, flag_single_quote, RESET);
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
		// printf("%stoken->type = %d%s\n", MAGENTA, token->type, RESET); ////////
	}
	ft_free_tab(words);
	return (head);
}
