when I execute this command:

I have :
minishell$> echo "test | yeah" | wc
minishell: yeah  :command not found
      3      10      71

I want:
minishell$> echo "test | yeah" | wc
      1       3      12

you need to split the command on the pipe only out the quote and ignore only pipe between the quote. Understand ?
t_command *get_command(char *input, char **envp)
{
	t_command	*head;
	t_command	*new_cmd;
	char		**command;
	int			i;

	head = NULL;
	if (is_char_inside_quotes('|', input))
	{
		command = (char **)malloc(sizeof(char *) * 2);
		command[0] = ft_strdup(input);
		command[1] = NULL;
	}
	else
		command = split_string(input, '|');
	i = 0;
	while (command[i])
	{
		if(ft_strcmp_minishell(command[i], "") == 0)
			return (NULL);
		new_cmd = create_new_cmd(command[i], envp);
		append_new_cmd(&head, new_cmd);
		i++;
	}
	ft_free_tab(command);
	return (head);
}
