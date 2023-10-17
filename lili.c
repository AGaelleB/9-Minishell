pourquoi jai ca comme erreur lors de lexec de ma commande:
minishell$> echo $PATH
coucou
munmap_chunk(): invalid pointer


char	*ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(ft_strlen(input) + 1);
	if (!(arg))
		exit(EXIT_FAILURE); // Handle memory allocation failure
	*arg_idx = 0;
	while (input[*i])
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if ((input[*i] == '>' || input[*i] == '<') && !double_quote
			&& !single_quote)
			break ;
		else if (input[*i] == ' ' && !double_quote && !single_quote)
			break ;
		else if (input[*i] == '$' && !single_quote)
		{
			char *str;
			int j = 0;
			int start = *i + 1; // +1 pour sauter le caractère '$'

			while (input[*i] && input[*i] != ' ' && input[*i] != '\'' && input[*i] != '\"')
			{
				(*i)++;
				j++;
			}

			str = ft_substr(input, start, j); // Utilisez ft_substr pour extraire la variable de l'environnement
			if (str)
			{
				char *value = env_var_exists(env, str);
				j = 0;
				if (value)
				{
					while (value[j])
					{
						arg[(*arg_idx)++] = value[j];
						j++;
					}
					printf("coucou\n");
					free(value);
				}
				free(str);
			}
		}

		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	ft_skip_redirection_and_file(input, i);
	return (arg);
}
