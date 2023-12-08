void handle_exit_status(char **str_cpy, int *y, int *i)
{
	char *exit_stat;
	int j = 0;

	exit_stat = ft_itoa(g_exit_status);
	while (exit_stat[j])
	{
		(*str_cpy)[(*y)++] = exit_stat[j++];
	}
	free(exit_stat);
	(*i)++;
}

void skip_var_name(char *str, int *i)
{
	(*i)++;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
}

char *check_none_var(char *str)
{
	char *str_cpy;
	int i = 0, y = 0;

	str_cpy = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!str_cpy)
		return (NULL);

	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				handle_exit_status(&str_cpy, &y, &i);
			else
				skip_var_name(str, &i);
		}
		else
		{
			str_cpy[y++] = str[i];
		}
		i++;
	}
	str_cpy[y] = '\0'; // Assurez-vous que la chaîne est bien terminée
	return (str_cpy);
}