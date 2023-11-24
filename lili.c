
can you help me to fix my code ? When I try to execute "minishell$> echo hola > bonjour_exit | cat -e bonjour" I have nothing, The problem seems to be inside cur->command_arg = parse_input_quote(cur->command);. 
when I do ft_free_tab(cur->command_arg);, after I can't execve my command. 


minishell$> echo hola > bonjour_exit | cat -e bonjour
minishell$> 


void	ft_set_args_and_paths(t_command *cur, t_env *env)
{
	int	i;

	if (cur->command_arg)
		ft_free_tab(cur->command_arg); // repare les leaks d une cmd not found mais casse des pipes
	
	cur->command_path = NULL;
	cur->command_arg = parse_input_quote(cur->command);

	i = 0;
	while (cur->command_arg[i++])
		printf("%scur->command_arg[%d] = %s%s\n", BLUE, i , cur->command_arg[i], RESET);
	
	cur->command_path = ft_check_paths(env, cur->command_arg[0]);
}

void	handle_quotes(char *input, int *i,
	bool *double_quote, bool *single_quote)
{
	char	quote_char;
	int		count;

	quote_char = input[*i];
	count = 0;
	if ((quote_char == '\"' || quote_char == '\'')
		&& !(*double_quote && *single_quote))
	{
		while (input[*i] == quote_char)
		{
			count++;
			(*i)++;
		}
		if (count % 2 != 0)
		{
			if (quote_char == '\"')
				*double_quote = !*double_quote;
			else
				*single_quote = !*single_quote;
		}
	}
}

char	*allocate_and_copy(char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(ft_strlen(input) + 1);
	if (!arg)
		return (NULL);
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes(input, i, &double_quote, &single_quote);
		if ((input[*i] == ' ' || input[*i] == '>' || input[*i] == '<')
			&& !double_quote && !single_quote)
			break ;
		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	return (arg);
}

void	skip_spaces(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

char	**copy_argument(char *input, t_parser *parser)
{
	char	*arg;
	int		arg_idx;

	arg = allocate_and_copy(input, &(parser->i), &arg_idx);
	if (!arg)
		return (NULL);
	if (arg_idx > 0)
		parser->args[(parser->idx)++] = arg;
	else
		free(arg);
	skip_spaces(input, &(parser->i));
	return (parser->args);
}

char	**parse_input_quote(char *input)
{
	t_parser	parser;
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input);
	parser.args = malloc(sizeof(char *) * (arg_count + 1));
	if (!parser.args)
		return (NULL);
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
