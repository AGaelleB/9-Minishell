t_token    *handle_cmd_token(t_tokenizer *tz, char **envp)
{
	char    *path;

	path = ft_check_paths(envp, tz->words[tz->i]);
	if (path)
	{
		if (!tz->cmd_processed)
		{
			tz->token = new_token(TYPE_CMD, tz->words[tz->i]);
			tz->cmd_processed = true; // Mark the command as processed
		}
		else
			tz->token = new_token(TYPE_ARG, tz->words[tz->i]);
		tz->state = TYPE_ARG;  // Set the state to TYPE_ARG immediately
		free(path);
	}
	return (tz->token);
}