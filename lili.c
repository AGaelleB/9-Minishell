il semblerait que la fonction qui tokenize ne split pas bien sur les espaces et associe a un type: 0 les espaces alors qu ils ne devraient pas etre associes.
comment resoudre ce probleme ?




typedef enum e_token_type
{
	TYPE_CMD, // 0				// cat 			de cat celine.txt
	TYPE_ARG, // 1				// celine.txt 	de cat celine.txt
	TYPE_SEPARATOR, // 2		// "|" 
	TYPE_REDIR_OUT, // 3		// ">"
	TYPE_REDIR_IN, // 4			// "<"
	TYPE_DELIMITATOR, // 5		// "<<"
	TYPE_REDIR_APPEND, // 6		// ">>"
	TYPE_F_OUT, // 7
	// TYPE_BUILTIN,
	// TYPE_HEREDOC,
	// TYPE_EOF
} t_e_token_type;

t_token *new_token(t_e_token_type e_type, char *split_value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);

	token->type = e_type;
	token->split_value = ft_strdup(split_value);  // Remember to free this later!
	token->next = (NULL);
	// printf("value : %s, type: %d\n", token->split_value, token->type);


	return (token);
}

t_token *tokenize_input(char *input)
{
	char		**words;
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	int			i;

	words = split_string(input, ' ');
	head = NULL;
	curr = NULL;
	i = 0;
	while (words[i])
	{
		token = NULL;
		if (ft_strcmp_minishell(words[i], "|") == 0)
			token = new_token(TYPE_SEPARATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], ">") == 0)
			token = new_token(TYPE_REDIR_OUT, words[i]);
		else if (ft_strcmp_minishell(words[i], "<") == 0)
			token = new_token(TYPE_REDIR_IN, words[i]);
		else if (ft_strcmp_minishell(words[i], ">>") == 0)
			token = new_token(TYPE_DELIMITATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], "<<") == 0)
			token = new_token(TYPE_REDIR_APPEND, words[i]);
		else if (i == 0 ||
				(i > 0 && (ft_strcmp_minishell(words[i - 1], "|") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<") == 0 ||
							ft_strcmp_minishell(words[i - 1], ">>") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<<") == 0)))
			token = new_token(TYPE_CMD, words[i]);
		
		else if(i > 0 && ft_strcmp_minishell(words[i - 1], ">") == 0)
			token = new_token(TYPE_F_OUT, words[i]);
		
		else
			token = new_token(TYPE_ARG, words[i]);
		if (!head)
		{
			head = token;
			curr = head;
		}
		else
		{
			curr->next = token;
			curr = token;
		}
		i++;
	}
	ft_free_tab(words);
	// ft_free_tokens(head);
	return (head);
}


./minishell
minishell$> cat test.txt | rev | rev  | wc -l > coucou | rev
Command 0: cat test.txt 
        command_arg: cat, type: 0 			// type 0
        command_arg: test.txt, type: 1		// type 1

Command 1:  rev 
        command_arg: , type: 0				// ? correspond au espaces " " ?
        command_arg: rev, type: 1			// type 0

Command 2:  rev  
        command_arg: , type: 0				// ? correspond au espaces " " ? 
        command_arg: rev, type: 1			// type 0
        command_arg: , type: 1				// ? 

Command 3:  wc -l > coucou 
        command_arg: , type: 0				// ? correspond au espaces " " ?
        command_arg: wc, type: 1			// type 0
        command_arg: -l, type: 1			// type 0
        command_arg: >, type: 3				// type 3
        command_arg: coucou, type: 7		// type 7

Command 4:  rev
        command_arg: , type: 0				// ? correspond au espaces " " ?
        command_arg: rev, type: 1			// type 0



autre exemple : 

./minishell
minishell$>  cat test.txt |             rev | rev                 | wc -l >             coucou | rev
Command 0:  cat test.txt 
        command_arg: , type: 0
        command_arg: cat, type: 1
        command_arg: test.txt, type: 1

Command 1:              rev 
        command_arg: , type: 0
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: rev, type: 1

Command 2:  rev                 
        command_arg: , type: 0
        command_arg: rev, type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1

Command 3:  wc -l >             coucou 
        command_arg: , type: 0
        command_arg: wc, type: 1
        command_arg: -l, type: 1
        command_arg: >, type: 3
        command_arg: , type: 7
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: , type: 1
        command_arg: coucou, type: 1

Command 4:  rev
        command_arg: , type: 0
        command_arg: rev, type: 1




int state = TYPE_CMD;  // Start by expecting a command initially

while (words[i])
{
	if (is_empty_or_space(words[i]))
	{
		i++;
		continue;
	}

	token = NULL;
	if (ft_strcmp_minishell(words[i], "|") == 0)
	{
		token = new_token(TYPE_SEPARATOR, words[i]);
		state = TYPE_CMD;  // Expect a command next
	}
	else if (ft_strcmp_minishell(words[i], ">") == 0)
	{
		token = new_token(TYPE_REDIR_OUT, words[i]);
		state = TYPE_F_OUT;  // Expect a filename next
	}
	// ... handle other redirection symbols similarly ...
	else if (state == TYPE_CMD)
	{
		token = new_token(TYPE_CMD, words[i]);
		state = TYPE_ARG;  // Expect arguments next
	}
	else
	{
		token = new_token(TYPE_ARG, words[i]);
	}

	// ... rest of your code ...
}
