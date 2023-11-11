






int handle_multiple_heredocs_two(t_command *current, t_token *token) {
    char *delimiter;
    int fd[2]; // Pipe file descriptors

    while (token && ft_strcmp_minishell(token->split_value, "<<") == 0) {
        delimiter = epur_filename_heredoc(current->token_head);

        if (pipe(fd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            close(fd[0]); // Close the read end
            write_in_fd(fd[1], delimiter, current); // Write to the pipe
            close(fd[1]); // Close the write end after writing
            exit(EXIT_SUCCESS);
        } else { // Parent process
            close(fd[1]); // Close the write end
            current->fd_in = fd[0]; // Use the read end as stdin for the command
        }

        if (delimiter) free(delimiter);
        token = token->next->next;
    }
    return 0; // Return 0 to indicate success
}

static void handle_heredoc_tokens(t_process_data *data) {
    t_token *token;
    int status;

    token = data->current->token_head;
    while (data->current)
	{
        while (token)
		{
            if (token->type == TYPE_HEREDOC){
                heredoc_open_fd_pipe(data->current, &token);
            }
            token = token->next;
        }
        data->current = data->current->next;
    }
    // Attendez tous les processus enfants ici
    for (int i = 0; i <= data->current->nb_pipes; i++) {
        waitpid(-1, &status, 0);
    }
}