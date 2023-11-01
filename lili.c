



char	*ft_allocate_and_copy(t_env *env, char *input, int *i,
	int *arg_idx)
{
	char	*arg;
	int		len;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;

	len = count_arg_length(input, *i); 
	arg = malloc(len + 1);
	if (!arg)
		exit(EXIT_FAILURE);
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if (is_redirection(input[*i]) && !double_quote && !single_quote)
		{
			ft_skip_redirection_and_file(input, i);
			continue;
		}
		else if (input[*i] == ' ' && !double_quote && !single_quote)
			break ;
		else if (input[*i] == '$' && !single_quote)
			handle_arg_value(env, input, i, arg, arg_idx);
		else
		{
			arg[(*arg_idx)++] = input[*i];
			(*i)++;
		}
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	return (arg);
}