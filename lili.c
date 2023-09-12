Nous voudrions implementer la gestion des commandes CTRL C (renvoie un nouveau prompt) et CTRL D (quitte le prompt proprement)
Pour cela nous voulons utiliser une variable globale "volatile int	g_unblock_sigquit". 
Peux tu m aider a implementer cela ? 

pour le moment lors de l exection de mon programme j'ai : 
le segfault correspond a la commande ctrl D


minishell$> ls | wc
      9       9      72
minishell$> 
minishell$> ^C
minishell$> ^C
minishell$> 
[1]    66235 segmentation fault (core dumped)  ./minishell

voici mon code : 

typedef struct s_sigset
{

} t_sigset;

volatile int	g_unblock_sigquit = 0;

void	block_signal(int signal)
{
	t_sigset	sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, signal);
//	Ajouter les signaux de l'ensemble aux signaux bloqués pour ce processus
	if (signal == SIGQUIT)
		printf("\e[36mSIGQUIT (ctrl-D) blocked.\e[0m\n");
}

// Fonction pour débloquer le signal spécifié
void	unblock_signal(int signal)
{
	t_sigset	sigset;

	sigemptyset(&sigset);
	sigaddset(&sigset, signal);
//	Retirer les signaux de l'ensemble des signaux bloqués pour ce processus
	if (signal == SIGQUIT)
		printf("\e[36mSIGQUIT (ctrl-\\) unblocked.\e[0m\n");
}

void handle_sigint (int signal) 
{
	if (signal != SIGINT)
		return 
	write(1, "\nminishell$> ", 13);
	block_signal(SIGINT);
	g_unblock_sigquit = 1;
	unblock_signal(SIGINT);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_command	*new_commands;

	new_commands = NULL;
	signal(SIGINT, handle_sigint);
	while (1)
	{
		input = readline("minishell$> ");
		if (ft_strcmp_minishell(input, "") == 0)
		{
			main(ac, av, envp);
			break;
		}
		if (ft_all_builtins(input) != 0)
		{
			free(input);
			exit(0);
		}
		new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			new_commands->token = tokenize_input(input);
			open_fd(new_commands, envp);
		}
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}

void child_process(t_command *current, int read_fd, int write_fd, char **envp)
{
    dup2(read_fd, 0);  // Duplique read_fd vers stdin
    if (write_fd != 1)
        dup2(write_fd, 1);  // Duplique write_fd vers stdout
    ft_set_args_and_paths(current, envp);
    if (current->command_path == NULL)
    {
        write(2, "minishell: command not found: ", 31);
        write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
        write(2, "\n", 1);
        exit(127);  // Code de sortie standard pour "commande introuvable"
    }
    else if (execve(current->command_path, current->command_arg, envp) == -1) 
    {
        perror("Error");
        exit(-1);
    }
    
    // Dans le cas où execve échoue sans raison connue, sortez du processus enfant.
    exit(1);  // Vous pouvez utiliser un autre code de sortie si nécessaire
}
