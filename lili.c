
char **split_string_token(char *str, char **delimiters)
{
	int i;
	int j;
	int k;
	int token_count = 1; 
	int start;
	int token_size;
	int delim_len = 0;

	i = 0;
	while(str[i])
	{
		if (is_delimiter(&str[i], delimiters, &delim_len))
		{
			token_count += 2; // One for the token before and one for the delimiter
			i += delim_len;   // skip delimiter characters
		}
		else
		{
			i++;
		}
	}

	char **tokens = malloc((token_count + 1) * sizeof(char *));
	if (!tokens)
	{
		perror("malloc");
		return NULL;
	}

	start = 0;
	i = 0;
	j = 0;

	while(str[j])
	{
		if (is_delimiter(&str[j], delimiters, &delim_len) || str[j + 1] == '\0')
		{
			if (is_delimiter(&str[j], delimiters, &delim_len))
				token_size = j - start;
			else
				token_size = j - start + 1;

			tokens[i] = malloc(token_size + 1);
			if (!tokens[i])
			{
				perror("malloc");
				for (k = 0; k < i; k++)
					free(tokens[k]);
				free(tokens);
				return NULL;
			}
			strncpy(tokens[i], &str[start], token_size);
			tokens[i][token_size] = '\0';

			i++;

			// Storing delimiter as a token
			if (delim_len > 0)
			{
				tokens[i] = malloc(delim_len + 1);
				if (!tokens[i])
				{
					perror("malloc");
					for (k = 0; k < i; k++)
						free(tokens[k]);
					free(tokens);
					return NULL;
				}
				strncpy(tokens[i], &str[j], delim_len);
				tokens[i][delim_len] = '\0';

				i++;
				j += delim_len; // skip delimiter characters
				start = j;
			}
			else
			{
				j++;
				start = j;
			}
		}
		else
		{
			j++;
		}
	}
	tokens[i] = NULL;
	return (tokens);
}
