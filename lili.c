voici ce que jai fait mais le numbre de pipe final stocke dans la struct semble etre dans une zone memoire non initialisee

voici le resultat apres la modification du code ci-dessou
➜  minish git:(main) ✗ ./minishell
minishell$>  cat txt.txt | grep m | wc
nombre de pipes commande : 1
nombre de pipes commande : 2
nombre de pipes commande : 2
nombre de pipes final : 1728434128


t_token *tokenize_input(char *input)
{
	char		**words;
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	int			i;

	words = split_string(input, ' ');  // Assuming a space delimiter
	head = NULL;
	curr = NULL;
	i = 0;
	int total_pipes = 0;
	// printf("tokenize_input avant boucle\n");
	while (words[i])
	{
		// printf("tokenize_input avant boucle\n");
		token = NULL;

		// ft_count_pipes(words);

		// if (strcmp(words[i], "|") == 0)
		// {
				
		// }
		if (strcmp(words[i], "|") == 0)
		{
			token = new_token(TYPE_SEPARATOR, words[i]);
			total_pipes++;
			printf("nombre de pipes commande : %d\n", total_pipes);
		}
		else if (strcmp(words[i], ">") == 0)
			token = new_token(TYPE_REDIR_OUT, words[i]);
		else if (strcmp(words[i], "<") == 0)
			token = new_token(TYPE_REDIR_IN, words[i]);
		else if (strcmp(words[i], ">>") == 0)
			token = new_token(TYPE_REDIR_APPEND, words[i]);
		else if (i == 0 ||
				(i > 0 && (strcmp(words[i - 1], "|") == 0 || 
							strcmp(words[i - 1], ">") == 0 || 
							strcmp(words[i - 1], "<") == 0 || 
							strcmp(words[i - 1], ">>") == 0)))
			token = new_token(TYPE_CMD, words[i]);
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
	token->nb_pipes = total_pipes;
	printf("nombre de pipes commande : %d\n", token->nb_pipes);

	return (head);
}