pourquoi lorsque en entree de str je donne " 'gaga' "
en sortie dans dest jai 'gaga" alors que jaurais voulu 'gaga'
aide moi 

voici mon code 
char *handle_quotes_export(char *str)
{
	int i;
	int y;
	bool single_quote;
	bool double_quote;
	char *dest;

	i = 0;
	y = 0;
	single_quote = false;
	double_quote = false;
	dest = malloc(sizeof(str) * (ft_strlen(str) + 1));
	if (!dest)
		return (NULL);
	while (str[i])
	{
		if (!single_quote && str[i] == '\"' && str[i + 1] == '\"')
			i += 2;
		else if (!double_quote && str[i] == '\'' && str[i + 1] == '\'')
			i += 2;
		if (!double_quote && str[i] == '\'')
			single_quote = !single_quote;
		else if (!single_quote && str[i] == '\"')
			double_quote = !double_quote;
		if (!single_quote && str[i] == '\"')
			i++;
		else if (!double_quote && str[i] == '\'')
			i++;
		dest[y] = str[i];
		i++;
		y++;
	}
	dest[y] = '\0';
	return (dest);
}
