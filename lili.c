
char	*extract_filename_heredoc(char *cmd)
{
	int		j;
	bool	in_quote;
	bool	double_quote;
	char	*file_name;
	int		i;

	in_quote = false;
	double_quote = false;
	i = 0;
	file_name = malloc(sizeof(char) * (ft_strlen(cmd) + 1));
	if (!file_name)
		return (NULL);
	j = 0;
	while (cmd[i] != '<')
		i++;
	while (cmd[i] == '<' || cmd[i] == ' ')
		i++;
	while (cmd[i])
	{
		if (cmd[i] == '>' || cmd[i] == '<')
			break ;
		handle_quotes_heredoc(cmd, &i, &in_quote, &double_quote);
		if (!in_quote && !double_quote && cmd[i] == ' ')
		{
			printf("je break alors qu eje ne devrais pas \n");
			break ;
		}
		file_name[j++] = cmd[(i)++];
	}
	file_name[j] = '\0';
	return (file_name);
}
