Notre fonction est recursive pour retirer tous les chevrons et les arguments qui debutent notre commande. 

Notre probleme est que la fonction "is_redir_at_beginning" renvoie 'i' sans avoir fait sa recursivité des le premier tour. 

int	is_redir_at_beginning(char *input, int i)
{

	while (input[i] == ' ')
		i++;

	if (input[i] == '>' || input[i] == '<' || (input[i] == '>' && input[i + 1] == '>'))
	{
		if (input[i] == '>' || input[i] == '<')
			i++;
		else
			i += 2;
		while (input[i] == ' ')
			i++;
		while (input[i] != ' ')
		{
			printf("input[i] = %c\n", input[i]);
			i++;
		}
		is_redir_at_beginning(input, i);
	}
	return (i);
}

char **parse_input_quote(char *input)
{
	bool	in_quote;
	int		arg_count;
	int		arg_idx;
	int		idx;
	char	**args;
	char	*arg;
	int		i;

	i = 0;
	i = is_redir_at_beginning(input, i);
	printf("i = %d\n", i);
	arg_count = count_args(input);
	args = malloc((arg_count + 1) * sizeof(char *));
	in_quote = false;
	idx = 0;
	if (!args)
		return (NULL);
	while (input[i])
	{
		arg = malloc(ft_strlen(input) + 1);
		if (!arg)
			return (NULL);
		arg_idx = 0;
		while (input[i] && (in_quote || (input[i] != ' ' && input[i] != '>' && input[i] != '<')))
		{
			if (input[i] == '\'')
				in_quote = !in_quote;
			else
				arg[arg_idx++] = input[i];
			i++;
		}
		if ((!in_quote) && (input[i] == '>' || input[i] == '<'))
			break;
		arg[arg_idx] = '\0';
		if (arg_idx > 0)
			args[idx++] = arg;
		else
			free(arg);
		while (input[i] == ' ')
			i++;
	}
	args[idx] = NULL;
	return (args);
}

