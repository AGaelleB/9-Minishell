Those are my fonctions, do you see the error ? : 


t_token	*handle_multiple_heredocs(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	while (token && ft_strcmp_minishell(token->split_value, "<<") == 0)
	{
		delimiter = extract_filename_heredoc(current->command);
		// printf("delimiter = %s\n", delimiter);
		if (current->heredoc)
			free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
		add_to_heredocs_list(current, current->heredoc);
		write_in_fd(fd, delimiter, current);
		// close(fd); // AJOUT Gaga close the write fd
		fd = open(current->heredoc, O_RDONLY);
		current->fd_in = fd;
		if (current->fd_in == -1)
		{
			perror("minishell: EOF");
			exit(-1);
		}
		token = token->next->next;
	}
	return(token);
}


int	redirect_heredoc(t_command *current, t_token *token)
{
	char	*delimiter;
	int		fd;

	fd = -1;
	delimiter = token->next->split_value;
	if (fd == -1)
	{
		if (current->heredoc)
			free(current->heredoc);
		current->heredoc = create_heredoc();
		fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);
	}
	write_in_fd(fd, delimiter, current);
	fd = open(current->heredoc, O_RDONLY);
	current->fd_in = fd;
	if (current->fd_in == -1)
	{
		perror("minishell: EOF");
		exit(-1);
	}
	return (0);
}

int heredoc_open_fd_pipe(t_command *command, t_token **token)
{
    if (*token && (*token)->type == TYPE_HEREDOC)
    {
            handle_multiple_heredocs(command, *token);
            if (command->fd_in != -1)
            {
                dup2(command->fd_in, 0);
                close(command->fd_in);
            }
    }
    return 0;  // return 0 to signify success, you may need to adjust this based on your code.
}