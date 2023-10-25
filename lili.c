t_token *create_new_token()
{
	t_token *new_tok;

	new_tok = malloc(sizeof(t_token));
	if (!new_tok)
		return (NULL);
	
	new_tok->next = NULL;
	new_tok->prev = NULL;
	// Initialisez les autres champs du token comme vous le faites actuellement...
	
	return new_tok;
}
