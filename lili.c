pourquoi je n'imprime pas la ligne: 	printf("curent :%s\n", current->command);

mon shell :
minishell$> echo "cououc" > "ton            shell"
OUI ??

ma strcuct :
typedef struct s_command			t_command;

typedef struct s_parser
{
	char	**args;
	int		i;
	int		idx;
	bool	in_quote;
} t_parser;


typedef struct s_quote 
{
	char	*str;
	char	**args;
	struct	s_quote  *next;
} t_quote;

typedef enum e_token_type
{
	TYPE_CMD, // 0				// cat 			de cat celine.txt
	TYPE_ARG, // 1				// celine.txt 	de cat celine.txt
	TYPE_SEPARATOR, // 2		// "|" 
	TYPE_SINGLE_QUOTE, // 3		// ' '
	TYPE_REDIR_OUT, // 4		// ">"
	TYPE_REDIR_IN, // 5			// "<"
	TYPE_REDIR_APPEND, // 6		// ">>"
	TYPE_HEREDOC, // 7			// "<<"
	TYPE_F_OUT, // 8
	TYPE_F_IN, // 9
	TYPE_EOF, // 10				// eof
	TYPE_FILENAME, // 11			

} t_e_token_type;

typedef struct s_token
{
	t_e_token_type		type;
	char				*split_value; // e.g. "cat"
	struct s_token		*next;
	struct s_command	*command;

} t_token;


typedef struct s_split_token
{
	int					i;
	int					j;
	int					start;
	int					delim_len;
	int					len_to_copy;
} t_split_token;

typedef struct s_tokenizer
{
	t_token		*head;
	t_token		*curr;
	t_token		*token;
	char		**words;
	char		*delimiters[6];
	int			i;
	int			state;
	bool		flag_single_quote;
	bool		flag_double_quote;
} t_tokenizer;

typedef struct s_command
{
	int					nb_pipes;
	char				*command;	// e.g. "cat test.txt"
	char				**command_arg;	// e.g. "cat"
	char				*command_path;	// e.g. /usr/bin/cat/
	int					fd[2];
	int					fd_out;
	int					fd_in;
	char				*file_name;
	struct s_token		*token_head;
	struct s_quote		*quote_head;
	struct s_command	*next;
} t_command;

mon code :

int	add_space_file_name(t_command *current)
{
	printf("OUI ??\n");
	printf("curent :%s\n", current->command);
	return(0);
}

char	*clean_file_name(t_token *token)
{
	// bool in_quote;
	char *file_name;
	t_token *temp;
	int i;
	int j;
	int k;
	
	i = 0;
	j = 0;
	k = 0;
	// in_quote = false;
	file_name = malloc(sizeof(char*)*(1000));
	temp = token->next;
	while(temp)
	{
		while(temp->split_value[i] != '\0')
		{
			if (temp->split_value[i] == '\'' || temp->split_value[i] == '"')
				i++;
			if(k == 1)
			{	
				k = 0;
				file_name[j] = ' ';
				j++;
			}
			add_space_file_name(token->command);
			file_name[j] = temp->split_value[i];
			i++;
			j++;
		}
		i = 0;
		k++;
		temp = temp->next;
	}
	return (file_name);
}
