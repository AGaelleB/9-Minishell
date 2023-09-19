je possede deux fonction qui parse ma commande en plusieurs blocs nommés command, command_arg, split_value, type.
finalement ils font un pue la meme chose split value et command_agrc stock la command mot par mot,
je voudrais arriver a un resultat qui fusionne ces deux fonctions.

cat celine.txt | rev | wc > test.txt

je veux;
dans la liste chainée dans command:  (jai deja ce resultat dans la fonction get_command)
command 0: cat celine.txt
command 1: rev
command 2: wc > test.txt

a linterieur des ces command utiliser les token pour avoir les arguments detaillés:
command_arg : cat, type: 0
command_arg : celine.txt, type: 1
command_arg : |, type: 2
command_arg : rev, type: 0
command_arg : |, type: 2
command_arg : wc, type: 0
command_arg : >, type: 3
command_arg : txt.txt, type: 7

on ne veut plus forcement avoir split_value.
Si ce nest pas necessaire, est plus simple de remanier les structures existantes pour n'en avoir plus qu'une ?

typedef enum e_token_type
{
	TYPE_CMD,
	TYPE_ARG,
	TYPE_SEPARATOR,
	TYPE_REDIR_OUT,		// ">"
	TYPE_REDIR_IN,		// "<"
	TYPE_DELIMITATOR,	// "<<"
	TYPE_REDIR_APPEND,	// ">>"
	TYPE_F_OUT,
	// TYPE_BUILTIN,
	// TYPE_HEREDOC,
	// TYPE_EOF
} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
	struct s_token		*before;
} t_token;

typedef struct s_sigset
{

} t_sigset;

typedef struct s_command
{
	int					nb_pipes;
	char				*command;	// e.g. "cat test.txt"
	char				**command_arg;	// e.g. "cat"
	char				*command_path;	// e.g. /usr/bin/cat/
	int					fd[2];
	int					fd_out;
	struct s_token		*token;
	struct s_command	*next;	// Pointeur vers la commande suivante
} t_command;


t_token *new_token(t_e_token_type e_type, char *split_value)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);

	token->type = e_type;
	token->split_value = ft_strdup(split_value);  // Remember to free this later!
	token->next = (NULL);
	printf("value : %s, type: %d\n", token->split_value, token->type);


	return (token);
}

t_token *tokenize_input(char *input)
{
	char		**words;
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	int			i;

	words = split_string(input, ' ');
	head = NULL;
	curr = NULL;
	i = 0;
	while (words[i])
	{
		token = NULL;
		if (ft_strcmp_minishell(words[i], "|") == 0)
			token = new_token(TYPE_SEPARATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], ">") == 0)
			token = new_token(TYPE_REDIR_OUT, words[i]);
		else if (ft_strcmp_minishell(words[i], "<") == 0)
			token = new_token(TYPE_REDIR_IN, words[i]);
		else if (ft_strcmp_minishell(words[i], ">>") == 0)
			token = new_token(TYPE_DELIMITATOR, words[i]);
		else if (ft_strcmp_minishell(words[i], "<<") == 0)
			token = new_token(TYPE_REDIR_APPEND, words[i]);
		else if (i == 0 ||
				(i > 0 && (ft_strcmp_minishell(words[i - 1], "|") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<") == 0 ||
							ft_strcmp_minishell(words[i - 1], ">>") == 0 ||
							ft_strcmp_minishell(words[i - 1], "<<") == 0)))
			token = new_token(TYPE_CMD, words[i]);
		else if(i > 0 && ft_strcmp_minishell(words[i - 1], ">") == 0)
			token = new_token(TYPE_F_OUT, words[i]);
		else
			token = new_token(TYPE_ARG, words[i]);
		if (!head)
		{
			head = token;
			curr = head;
		}
		else
		{
			curr->next = token;
			curr = token;
		}
		i++;
	}
	ft_free_tab(words);
	// ft_free_tokens(head);
	return (head);
}

t_command	*get_command(char *input)
{
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;
	char		**command;
	int			i;

	head = NULL;
	current = NULL;
	command = split_string(input, '|');
	i = 0;
	while(command[i])
	{
		new_cmd = malloc(sizeof(t_command));
		if (!new_cmd)
		{
			perror("Failed to allocate memory for new command");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->command = NULL;
		new_cmd->command = ft_strdup(command[i]);
		printf("command %d: %s\n", i, new_cmd->command);
		if (!new_cmd->command)
		{
			perror("Failed to duplicate command string");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->next = NULL;
		if (!head)
		{
			head = new_cmd;
			current = head;
		}
		else
		{
			current->next = new_cmd;
			current = new_cmd;
		}
		i++;
	}
	ft_free_tab(command);
	return (head);
}


int main(int ac, char **av, char **envp)
{
	char		*input;
	int			builtin_status;
	t_command	*new_commands;
	
	new_commands = NULL;
	signal(SIGINT, ft_signal_ctrl_C);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		input = readline("minishell$> ");
		ft_builtin_ctrl_D(input);
		builtin_status = ft_all_builtins_exit(input);
		if (builtin_status == 1)
		{
			free(input);
			exit(0);
		}
		else if (builtin_status == 2)
			continue;
		new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			new_commands->token = tokenize_input(input);
			// printf("%s\n %d\n\n", new_commands->token->split_value, new_commands->token->type);
			// if(new_commands->token != NULL)
				execve_fd(new_commands, envp);
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