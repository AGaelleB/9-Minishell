Cela s ameliore un peu, je n'ai plus que 3 fichiers temporaire en fin d execution mais le dernier heredoc ne saffiche plus correctement

minishell$> cat << a << b << c
Début de handle_multiple_heredocs
Traitement du token heredoc : <<
Délimiteur pour ce token : a
Création du fichier heredoc : zkezxqpsvf
Création du fichier heredoc : zkezxqpsvf
> coucou a
> a
Traitement du token heredoc : <<
Délimiteur pour ce token : b
Création du fichier heredoc : jyqqhycysu
Création du fichier heredoc : jyqqhycysu
> coucou b
> b
Traitement du token heredoc : <<
Délimiteur pour ce token : c
Création du fichier heredoc : chvhgurcat
Création du fichier heredoc : chvhgurcat
> coucou c
> c
minishell$> 


je veux : 

minishell$>  cat << a << b << c
> coucou a
> a
> coucou b
> b
> coucou c
> c
coucou c


voici mes modifications :

t_token	*handle_multiple_heredocs(t_command *current, t_token *token) // NEWWWW
{
    char	*delimiter;
    int		fd;

    printf("Début de handle_multiple_heredocs\n");

    while (token && ft_strcmp_minishell(token->split_value, "<<") == 0)
    {
        printf("Traitement du token heredoc : %s\n", token->split_value);

        delimiter = token->next->split_value;

        printf("Délimiteur pour ce token : %s\n", delimiter);

        if (current->heredoc)
            free(current->heredoc);
        current->heredoc = create_heredoc();
        printf("Création du fichier heredoc : %s\n", current->heredoc);
        fd = open(current->heredoc, O_CREAT | O_EXCL | O_RDWR, 0644);

        write_in_fd(fd, delimiter);
        fd = open(current->heredoc, O_RDONLY);
        current->fd_in = fd;

        if (current->fd_in == -1)
        {
            perror("minishell: EOF");
            exit(-1);
        }

        token = token->next->next;
    }

    // printf("Fin de handle_multiple_heredocs\n");
    return(token); // ici
}


int	open_fd(t_command *command)
{
	t_token	*token;
	t_token	*token_head;

	token = command->token_head;
	token_head = command->token_head;
	while (token)
	{
		heredoc_open_fd(command, &token);
		redirect_file_in_open_fd(command, token, token_head);
		redirect_file_out_open_fd(command, token, token_head);
		redirect_append_file_out_open_fd(command, token, token_head);
		token = token->next;
	}
	// printf("open_fd = %s\n", command->heredoc);
	return (0);
}



void clean_heredoc_files(t_command *cur)
{
	t_command *tmp = cur;
	while (tmp)
	{
		if (tmp->heredoc)
		{
			printf("Tentative de suppression du fichier heredoc : %s\n", tmp->heredoc); // PRINNNT
			unlink(tmp->heredoc);
			free(tmp->heredoc);
			tmp->heredoc = NULL;
		}
		tmp = tmp->next;
	}
}


int	execve_process(t_command *cur, t_env *env)
{
	// free_file_name(cur->file_name); // ne marche pas
	// unlink(cur->heredoc); // METTRE ICIII
	// free(cur->file_name); // utile ? 

	clean_heredoc_files(cur);
	
	ft_set_args_and_paths(cur, env);
	if (env->flag_error || is_builtin(cur) == 2)
		exit(0);
	if ((cur->command_path == NULL) && is_builtin(cur) == 0)
	{
		write(2, "minishell: ", 11);
		write(2, cur->command_arg[0], ft_strlen(cur->command_arg[0]));
		write(2, " :command not found", 19);
		write(2, "\n", 1);
		ft_free_tab(cur->command_arg);
		ft_free_current(cur);
		return (127);
	}
	else if ((cur->command_path)
		&& (execve(cur->command_path, cur->command_arg, env->cpy_env) == -1))
	{
		perror("Error");
		return (-1);
	}
	return (0);
}

void heredoc_open_fd(t_command *command, t_token **token)
{
    if (*token && (*token)->type == TYPE_HEREDOC)
    {
        *token = handle_multiple_heredocs(command, *token);
        if (command->fd_in != -1)
        {
            dup2(command->fd_in, 0);
            close(command->fd_in);
        }
    }
}

int	open_fd(t_command *command)
{
	t_token	*token;
	t_token	*token_head;

	token = command->token_head;
	token_head = command->token_head;
	while (token)
	{
		heredoc_open_fd(command, &token);
		redirect_file_in_open_fd(command, token, token_head);
		redirect_file_out_open_fd(command, token, token_head);
		redirect_append_file_out_open_fd(command, token, token_head);
		token = token->next;
	}
	// printf("open_fd = %s\n", command->heredoc);
	return (0);
}
