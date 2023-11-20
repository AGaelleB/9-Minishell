
void handle_single_quote(t_arg_handler *arg_handler)
{
	char *str = arg_handler->input;
	int *i = arg_handler->i;
	bool *single_quote = arg_handler->single_quote;
	bool *double_quote = arg_handler->double_quote;

	if (str[*i] == '\'' && !*double_quote && str[*i + 1] != '\'')
	{
		*single_quote = !*single_quote;
		(*i)++;
	}
}

void handle_double_quote(t_arg_handler *arg_handler)
{
	char *str = arg_handler->input;
	int *i = arg_handler->i;
	bool *single_quote = arg_handler->single_quote;
	bool *double_quote = arg_handler->double_quote;

	if (str[*i] == '\"' && !*single_quote && str[*i + 1] != '\"')
	{
		*double_quote = !*double_quote;
		(*i)++;
	}
}

void skip_consecutive_quotes(t_arg_handler *arg_handler, char quote_char)
{
	char *str = arg_handler->input;
	int *i = arg_handler->i;
	bool *quote_flag = (quote_char == '\'') ? arg_handler->single_quote : arg_handler->double_quote;

	while (!*quote_flag && str[*i] == quote_char && str[*i + 1] == quote_char)
		*i += 2;
}

void toggle_quote_state(t_arg_handler *arg_handler, char quote_char)
{
	char *str = arg_handler->input;
	int *i = arg_handler->i;
	bool *quote_flag = (quote_char == '\'') ? arg_handler->single_quote : arg_handler->double_quote;
	bool *opposite_quote_flag = (quote_char == '\'') ? arg_handler->double_quote : arg_handler->single_quote;

	if (!*opposite_quote_flag && str[*i] == quote_char)
		*quote_flag = !*quote_flag;
}

void advance_index_if_quote(t_arg_handler *arg_handler, char quote_char)
{
	char *str = arg_handler->input;
	int *i = arg_handler->i;
	bool *quote_flag = (quote_char == '\'') ? arg_handler->single_quote : arg_handler->double_quote;

	if (!*quote_flag && str[*i] == quote_char)
		(*i)++;
}

void handle_quotes_echo(t_arg_handler *arg_handler)
{
	handle_single_quote(arg_handler);
	handle_double_quote(arg_handler);
	skip_consecutive_quotes(arg_handler, '\"');
	skip_consecutive_quotes(arg_handler, '\'');
	toggle_quote_state(arg_handler, '\'');
	toggle_quote_state(arg_handler, '\"');
	advance_index_if_quote(arg_handler, '\'');
	advance_index_if_quote(arg_handler, '\"');
}
