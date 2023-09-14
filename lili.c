Je ne veux pas que "^\" soit affiché sur le terminal lorsque je l'execute dans mon prompte> Je veux qu'il ne se passe rien

./minishell 
minishell$> ^\

void ft_signal_backslash(int signal)
{
	(void)signal;
	rl_on_new_line();  // Informe readline que nous sommes sur une nouvelle ligne
	rl_replace_line("", 0);  // Efface la ligne actuelle
	rl_redisplay();  // Réaffiche le prompt
}

int main(int ac, char **av, char **envp)
{
	char		*input;
	int			builtin_status;
	
	signal(SIGINT, ft_signal_ctrl_C);
	signal(SIGQUIT, ft_signal_backslash);
	while (1)
	{
		input = readline("minishell$> ");
		ft_builtin_ctrl_D(input);
		builtin_status = ft_all_builtins_exit(input);
		if (builtin_status == 1)
		{
			free(input);
			cleanup_resources();
			exit(0);
		}
		else if (builtin_status == 2)
			continue;
		new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			new_commands->token = tokenize_input(input);
			open_fd(new_commands, envp);
		}
		add_history(input);
		ft_free_current(new_commands);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}