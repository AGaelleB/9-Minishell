
char	*epur_filename(t_token *token_head)
{
	char *file_name;
	char *tempo;
	int i;
	int j;
	bool in_quote;
	bool double_quote;
	size_t cmd_len;

	i = 0;
	j = 0;
	file_name = NULL;
	in_quote = false;
	double_quote = false;
	cmd_len = ft_strlen(token_head->command);

	file_name = malloc((cmd_len + 1) * sizeof(char));
	if (!file_name)
		return (NULL);
	// file_name = malloc(sizeof(char*)*(1000));
	while(token_head->command[i] != '>')
		i++;
	while((token_head->command[i] != '\'') && (token_head->command[i] != '\"'))
		i++;
	while(token_head->command[i])
	{
		if (token_head->command[i] == '>' || token_head->command[i] == '<')
			break;
		if (token_head->command[i] == '\'')
			in_quote = !in_quote;
		if (token_head->command[i] == '\"')
			double_quote = !double_quote;
		if (!in_quote && (token_head->command[i] == '\"'))
			i++;
		if (!double_quote && (token_head->command[i] == '\''))
			i++;
		if(!double_quote && !in_quote)
			break;
		file_name[j] = token_head->command[i];
		i++;
		j++;
	}
	file_name[j] = '\0';
	tempo = malloc(sizeof(char*)*(1000));;
	j = 0;
	while(token_head->command[i])
	{
		tempo[j] = token_head->command[i];
		i++;
		j++;
	}
	token_head->command = tempo;
	return(file_name);
}