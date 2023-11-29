char	**parse_arg_export(char *input) //////////
{
	t_parser	parser;
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input);
	parser.args = malloc((arg_count + 1) * sizeof(char *));
	if (!parser.args)
		return (NULL);
	parser.in_quote = false;
	parser.idx = 0;
	while (input[parser.i])
	{
		parser.args = copy_argument_export(input, &parser);
		if (!parser.args)
			return (NULL);
		if ((!parser.in_quote) && (input[parser.i] == '>'
				|| input[parser.i] == '<'))
			break ;
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}

char	**parse_input_quote_echo(t_env *env, char *input)
{
	t_parser	parser;
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input);
	parser.args = malloc((arg_count + 1) * sizeof(char *));
	if (!parser.args)
		return (NULL);
	parser.in_quote = false;
	parser.idx = 0;
	while (input[parser.i])
	{
		parser.args = copy_argument_echo(env, &parser, input);
		if (!parser.args)
			return (NULL);
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}

char	**parse_input_quote(char *input)
{

	t_parser	parser;
	int			arg_count;

	arg_count = count_args_single_quotes(input);
	parser.args = malloc(sizeof(char *) * (arg_count + 1));
	if (!parser.args)
		return (NULL);
	parser.i = is_redir_at_beginning(input, 0);
	parser.in_quote = false;
	parser.idx = 0;
	while (input[parser.i])
	{
		parser.args = copy_argument(input, &parser);
		if (!parser.args)
			return (NULL);
		if ((!parser.in_quote) && (input[parser.i] == '>'
				|| input[parser.i] == '<'))
			break ;
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}
