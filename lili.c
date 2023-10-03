J'ai un probleme avec ma fonction tokenize_input, la gsetion des redirections dans les entre single quote '\'', j'ai voulu utiliser un bool flag_single_quote afin de differencier les arguments entre single quote.unix
Cependant lors de l execution si ma commande est "cat celine.txt | echo 'coucou>t'est" je crée un doc "t'est"

MINIHELL$> cat celine.txt | echo 'coucou>t'est>a>b>c
words[0]: cat
words[1]:  
words[2]: celine.txt
words[3]:  
words[0]: 
words[1]:  
words[2]: echo
words[3]:  
words[4]: 'coucou
words[5]: >
words[6]: t'est
words[7]:  
words[8]: 
words[9]: >
words[10]: 
words[11]:  
words[12]: a
words[13]:  
words[14]: 
words[15]: >
words[16]: 
words[17]:  
words[18]: b
words[19]:  
words[20]: 
words[21]: >
words[22]: 
words[23]:  
words[24]: c
valid_path = /usr/bin/echo
Command 0:  echo 'coucou>t'est > a > b > c
        command_arg: echo, type: 0
        command_arg: 'coucou, type: 1
        command_arg: >, type: 4
        command_arg: t'est, type: 8
        command_arg: >, type: 4
        command_arg: a, type: 8
        command_arg: >, type: 4
        command_arg: b, type: 8
        command_arg: >, type: 4
        command_arg: c, type: 8

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
			flag_single_quote = !flag_single_quote;
			token = new_token(TYPE_ARG, words[i]);
			state = TYPE_ARG;
		}
		else if ((flag_single_quote) && (ft_strcmp_minishell(words[i], ">") == 0))
		{
			token = new_token(TYPE_REDIR_OUT, words[i]);
			state = TYPE_F_OUT;
		}
		else if ((flag_single_quote) && (ft_strcmp_minishell(words[i], "<") == 0))
		{
			token = new_token(TYPE_REDIR_IN, words[i]);
			state = TYPE_F_IN;
		}
		else if ((flag_single_quote) && (ft_strcmp_minishell(words[i], ">>") == 0))
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
