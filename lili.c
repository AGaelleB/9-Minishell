static char *initialize_arg_and_vars(t_arg_handler *arg_handler, bool *double_quote,
	bool *single_quote, int *arg_idx)
{
	char *arg;
	int size_of_argument;

	size_of_argument = calculate_size_of_argument(arg_handler->input);
	*double_quote = false;
	*single_quote = false;
	arg = malloc(sizeof(char) * (size_of_argument + 1));
	if (!arg)
		exit(EXIT_FAILURE);
	*arg_idx = 0;
	return arg;
}

static void process_input(t_arg_handler *arg_handler,
	char *arg, int *arg_idx, bool *double_quote, bool *single_quote)
{
	int *i = arg_handler->i;

	while (arg_handler->input[*i] && (*double_quote || *single_quote \
		|| arg_handler->input[*i] != ' '))
	{
		handle_quotes_echo(arg_handler);
		if (is_redirection(arg_handler->input[*i]) && !*double_quote && !*single_quote)
			ft_skip_redirection_and_file(arg_handler->input, i);
		else if (arg_handler->input[*i] == '$' && !*single_quote)
			handle_arg_value(arg_handler);
		else
			arg[(*arg_idx)++] = arg_handler->input[(*i)++];
	}
	arg[*arg_idx] = '\0';
}

char *ft_allocate_and_copy(t_arg_handler *arg_handler)
{
	char *arg;
	bool double_quote;
	bool single_quote;
	int arg_idx;

	arg = initialize_arg_and_vars(arg_handler, &double_quote, &single_quote, &arg_idx);
	arg_handler->arg = arg;
	arg_handler->double_quote = &double_quote;
	arg_handler->single_quote = &single_quote;
	process_input(arg_handler, arg, &arg_idx, &double_quote, &single_quote);
	skip_spaces_echo(arg_handler->input, arg_handler->i);
	return arg;
}
