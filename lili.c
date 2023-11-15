int    here_doc_manage(t_process_data *data, int fd[2], char *delimiter)
{
    char    *input;
    int        i;
    int        result;

    (void)data;
    i = 0;
    signal(SIGINT, ctrl_c_heredoc);
    close(fd[0]);
    while (1)
    {
        input = readline("> ");
        result = handle_input(input, i, delimiter, fd);
        if (result != 0)
            return (result);
        i++;
    }
    signal(SIGQUIT, ctrl_c_heredoc);
    return (0);
}

int    here_doc_ray(t_process_data *data)
{
    int    i;
    int    heredoc_interrupted;

    data->count_hd = count_heredocs(data->current->token_head);
    data->heredocs = malloc(sizeof(t_here_doc) * data->count_hd);
    if (!data->heredocs)
        return (-1);
    signal(SIGINT, ctrl_c_heredoc);
    i = 0;
    heredoc_interrupted = 0;
    while (i < data->count_hd && !heredoc_interrupted)
    {
        heredoc_interrupted = manage_single_heredoc(data, i);
        i++;
    }
    signal(SIGQUIT, ctrl_c_heredoc);
    if (heredoc_interrupted)
        return (-1);
    else
        return (0);
}