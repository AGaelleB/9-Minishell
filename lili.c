
int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	t_env		*env_bis;
	char		*input;
	int			status;

	(void)av;
	env_bis = (t_env *)malloc(sizeof(t_env)); // A FREE
	if (!env_bis)
		return (1);
	if (isatty(0))
	{
		if (ac != 1)
			return (printf("run ./minishell without arg\n"));
		if (!envp[0])
			return (printf("env is missing\n"));
		g_exit_status = 0;
		// printf("Debug: g_exit_status set to %d at main\n", g_exit_status);
		signal(SIGINT, ft_builtin_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		copy_env(env_bis, envp);
		while (1)
		{
			// printf("begin main %d\n", g_exit_status);
			input = readline("minishell$> ");
			// printf("after readline main %d\n", g_exit_status);
			ft_builtin_ctrl_d(input);
			if (error_input(input) == 2 || verif_nb_quote(input) != 0 || pipe_syntax_errors(input) == -1)
			{
				// free(input); // invalid free ???
				continue;
			}
			add_history(input);
			new_commands = get_command(input, env_bis);
			count_and_set_pipes(input, new_commands);
			if (new_commands != NULL)
			{
				new_commands->command_arg = parse_input_quote(new_commands->command);
				execve_builtins_unset_export(new_commands, env_bis);
				execve_builtin_cd(new_commands, env_bis);
				pid_t pid = fork();
				if (pid == 0)
				{
					// printf("if main %d\n", g_exit_status);
					execve_fd(new_commands, env_bis);
					// exit(64); // permet de kill les process des childs mais bug 
					// printf("if after execve main %d\n", g_exit_status);
					// return (64);
					exit(g_exit_status);
				}
				else if (pid < 0)
					perror("fork");
				else
				{
					// printf("else main %d\n", g_exit_status);
					waitpid(pid, &status, 0); // La valeur de status est mise à jour ici
					if (WIFEXITED(status))
						g_exit_status = WEXITSTATUS(status); // Mettez à jour g_exit_status avec le code de sortie du processus
				}
				// g_exit_status = 42;
				ft_free_struct(new_commands, new_commands->token_head);
				ft_free_current(new_commands);
				free(input);
				// printf("end main %d\n", g_exit_status);
			}
		}
	}
	else
	{
		printf("the standard input is NOT from a terminal\n");
		return (-1);
	}
	return (0);
}

