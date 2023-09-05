











int	main(int ac, char **av, char **envp)
{
	char *input;

	while (1)
	{
		input = readline("minishell$> "); // utiliser GNL ? 

		if (!input)
		{
			// L'utilisateur a appuyé sur Ctrl+D (EOF) pour quitter.
			printf("exit\n");
			break;
		}
		
		// Vous pouvez ajouter ici la logique de traitement de la commande
		// Par exemple, vous pouvez utiliser des instructions conditionnelles pour exécuter différentes actions en fonction de la commande entrée.

		char **cmd_args = ft_split(input, ' ');
		char *cmd_path = ft_check_paths(envp, input);

		execve(cmd_path, cmd_args, envp);
		
		if (strcmp(input, "exit") == 0)
		{
			printf("exit\n");
			free(input);
			break;
		}
		else
			printf("%s\n", input);
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	return 0;
}
make re
