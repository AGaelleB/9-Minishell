
char	*allocate_and_initialize_dest(char *str)
{
	char	*dest;

	dest = malloc(sizeof(char) * SIZE);
	if (!dest)
		return (NULL);
	ft_memset(dest, '\0', ft_strlen(str) + 1);
	return (dest);
}

char	*handle_quotes_export(char *str)
{
	int		i;
	int		y;
	bool	single_quote;
	bool	double_quote;
	char	*dest;

	i = 0;
	y = 0;
	single_quote = false;
	double_quote = false;
	dest = allocate_and_initialize_dest(str);
	if (!dest)
		return (NULL);
	while (str[i])
	{
		skip_paired_quotes(str, &i, &single_quote, &double_quote);
		if (str[i] == '\0')
			break ;
		toggle_quote_flags(str[i], &single_quote, &double_quote);
		if (should_skip_char(str[i], single_quote, double_quote))
			i++;
		else
			dest[y++] = str[i++];
	}
	dest[y] = '\0';
	return (dest);
}
