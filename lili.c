int	count_args(char *input, int i)
{
	int		arg_count = 0;
	bool	double_quote = false;
	bool	single_quote = false;

	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!input[i])
			break;
		if (is_redirection(input[i]))
		{
			ft_skip_redirection_and_file(input, &i);
			continue;
		}
		arg_count++;
		while (input[i] && (double_quote || single_quote || input[i] != ' '))
		{
			handle_quotes_echo(input, &i, &double_quote, &single_quote);
			i++;
		}
	}
	return arg_count;
}

int	count_arg_length(char *input, int i)
{
	int		length = 0;
	bool	double_quote = false;
	bool	single_quote = false;

	while (input[i])
	{
		if (is_redirection(input[i]))
		{
			ft_skip_redirection_and_file(input, &i);
			continue;
		}
		while (input[i] && (double_quote || single_quote || input[i] != ' '))
		{
			length++;
			handle_quotes_echo(input, &i, &double_quote, &single_quote);
			i++;
		}
		while (input[i] == ' ')
			i++;
	}
	return length;
}

char	*ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
	char	*arg;
	int		len;
	bool	double_quote = false;
	bool	single_quote = false;

	len = count_arg_length(input, *i); 
	arg = malloc(len + 1);  // +1 for the null terminator
	if (!arg)
		exit(EXIT_FAILURE);
	*arg_idx = 0;

	while (input[*i])
	{
		// ... [rest of your function]
	}
	// ... [rest of your function]
	return arg;
}
