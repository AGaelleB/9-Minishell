/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:09:20 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/08 14:26:54 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status;

char	*allocate_and_copy_export(char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(ft_strlen(input) + 1);
	if (!arg)
		return (NULL);
	*arg_idx = 0;
	while (input[*i])
	{
		if (!double_quote && input[*i] == '\'')
			single_quote = !single_quote;
		else if (!single_quote && input[*i] == '\"')
			double_quote = !double_quote;
		if ((input[*i] == ' ' || input[*i] == '>' || input[*i] == '<')
			&& !double_quote && !single_quote)
			break ;
		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	return (arg);
}

/*Fonction 2: Gestion des espaces après un argument*/
void	skip_spaces_export(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

/*Fonction 3: Copie d'un argument dans le tableau d'arguments*/
char	**copy_argument_export(char *input, t_parser *parser)
{
	char	*arg;
	int		arg_idx;

	arg = allocate_and_copy_export(input, &(parser->i), &arg_idx);
	if (!arg)
		return (NULL);
	if (arg_idx > 0)
		parser->args[(parser->idx)++] = arg;
	else
		free(arg);
	skip_spaces_export(input, &(parser->i));
	return (parser->args);
}

/*Fonction 4 (principale): Parse Input avec Quotes*/
char	**parse_arg_export(char *input)
{
	t_parser	parser;
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input);
	parser.args = malloc((arg_count + 1) * sizeof(char *));
	parser.in_quote = false;
	parser.idx = 0;
	if (!parser.args)
		return (NULL);
	while (input[parser.i])
	{
		parser.args = copy_argument_export(input, &parser);
		if (!parser.args)
			return (NULL);
		if ((!parser.in_quote) && (input[parser.i] == '>'
				|| input[parser.i] == '<'))
			break ;
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}


int main(int ac, char **av, char **envp)
{
	t_command	*new_commands;
	t_env		*env_bis;
	char		*input;
	int			status;

	(void)av;
	if (isatty(0))
	{
		if (ac != 1)
			return (printf("run ./minishell without arg\n"));
		if (!envp[0])
			return (printf("env is missing\n"));
		g_exit_status = 0;
		signal(SIGINT, ft_builtin_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		env_bis = (t_env *)malloc(sizeof(t_env));
		if (!env_bis)
			return (1);
		copy_env(env_bis, envp);
		while (1)
		{
			input = readline("minishell$> ");
			ft_builtin_ctrl_d(input);
			ft_builtin_write_exit(input);
			if (error_input(input) == 2 || verif_nb_quote(input) != 0 || pipe_syntax_errors(input) == -1)
				continue;
			add_history(input);
			new_commands = get_command(input, env_bis);
			count_and_set_pipes(input, new_commands);
			if (new_commands != NULL)
			{
				new_commands->command_arg = parse_input_quote(new_commands->command);
				new_commands->export_arg = parse_arg_export(new_commands->command); //not free
				execve_builtins_unset_export(new_commands, env_bis);
				execve_builtin_cd(new_commands, env_bis);
				pid_t pid = fork();
				if (pid == 0)
				{
					execve_fd(new_commands, env_bis);
					ft_close_all_fd();
					ft_free_all(new_commands, new_commands->token_head);
					exit(g_exit_status);
				}
				else if (pid < 0)
					perror("fork");
				else
				{
					waitpid(pid, &status, 0);
					if (WIFEXITED(status))
						g_exit_status = WEXITSTATUS(status);
				}
			}
			free(input);
		}
		ft_close_all_fd();
		ft_free_env(env_bis);
		ft_free_all(new_commands, new_commands->token_head);
	}
	else
	{
		printf("the standard input is NOT from a terminal\n");
		return (-1);
	}
	return (0);
}


/*
										TO DO :

<<un <<deux cat | <<trois cat
=> n affiche pas le dernier heredoc, faire en sorte d ignorer si << apres pipe 

minishell$> export GAGA="'gaga'"
export GAGA="'gaga'"

minishell$> export = 
minishell: export: `=': not a valid identifier

EOF et ctrl^c leaks

 aie aie aie jai peur :
	minishell$> cat << " 'a' " << "b" << 'c'
	delimiter =  'a'  | current->heredoc = xguhpidyod 
	>  'a' 
	delimiter = b | current->heredoc = hyuuglvvda 
	> minishell: warning: here-document at line 0 delimited by end-of-file (wanted 'b')
	delimiter = c | current->heredoc = vbtsjdkezs 
	> minishell: warning: here-document at line 0 delimited by end-of-file (wanted 'c')

penser qa rechercher les truc quon a (void) et voir si utile. pareil pour forbiden function et a recoder

*/
