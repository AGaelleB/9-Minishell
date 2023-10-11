ca ne marche pas ou est mon erreur ? 

minishell$> ls || 
minishell: syntax error command required after '||'
minishell$> ls |     | 
minishell: syntax error command required after '|'
minishell: syntax error command required after '|'


int check_syntax_errors(char *input)
{
    // Vérifiez si "||" ou " | | " existe dans la chaîne d'entrée
    char *ptr = input;
    while ((ptr = strstr(ptr, "|")))
    {
        if (*(ptr + 1) == '|' || ( *(ptr + 1) == ' ' && *(ptr + 2) == '|'))
            return 1; // Erreur: "||" ou " | | " trouvé
        ptr++; // Passez au prochain caractère après "|"
    }
    return 0; // Aucune erreur trouvée
}


int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	char		*input;
	int			builtin_status;
	if (ac != 1)
		return (printf("run ./minishell without arg\n"));
	if (!envp[0])
		return (printf("env is missing\n"));
	signal(SIGINT, ft_builtin_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell$> ");
		// input = readline("😈🔥 MINIHELL$> ");
		ft_builtin_ctrl_d(input);
		builtin_status = ft_all_builtins(input);
		if (builtin_status == 1)
		{
			free(input);
			exit(0);
		}
		else if (builtin_status == 2)
			continue;
		if (verif_nb_quote(input) != 0)
			continue;
		add_history(input);
        if (check_syntax_errors(input))
        {
            ft_putstr_fd("minishell: syntax error command required after '||'\n", 2);
            free(input);
            continue;
        }
		new_commands = get_command(input, envp);
		if(new_commands == NULL)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token \'|\'\n", 2);
			free (input);
			continue;
		}
		count_and_set_pipes(input, new_commands);
		// ft_all_builtins_verif(new_commands);
		// print_commands_and_tokens(new_commands); // PRINT
		if(new_commands != NULL)
			execve_fd(new_commands, envp);

		// ft_free_tab(new_commands->command_arg);
		ft_free_struct(new_commands, new_commands->token_head);
		ft_free_current(new_commands);
		free(input);
	}
	(void)av;
	return (0);
}