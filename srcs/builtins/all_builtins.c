/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 10:02:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/13 10:14:43 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_all_builtins(char *input)
{
	if (ft_builtin_enter(input) != 0)
		return (2);
	if (ft_builtin_write_exit(input) != 0)
	{
		free(input);
		exit(0);
	}
	if (ft_is_all_space(input) != 0)
		return (2);
	if (ft_strcmp_minishell(input, "\"\"") == 0
			|| ft_strcmp_minishell(input, "\'\'") == 0)
	{
		printf("minishell: : command not found\n");
		free(input);
		return (2);
	}
	return (0);
}

/* extention of ft_allocate_and_copy*/
void	ft_handle_quotes(char *input, int *i,
	bool *double_quote, bool *single_quote)
{
	if (input[*i] == '\"' && !*single_quote)
	{
		*double_quote = !*double_quote;
		(*i)++;
	}
	else if (input[*i] == '\'' && !*double_quote)
	{
		*single_quote = !*single_quote;
		(*i)++;
	}
}

/*Fonction 1: Gestion des espaces après un argument*/
void	ft_skip_spaces(char *input, int *i)
{
	while (input[*i] == ' ')
		(*i)++;
}

/*Fonction 2: Allocation de l'argument et copie des caractères*/
// char	*ft_allocate_and_copy(char *input, int *i, int *arg_idx)
// {
// 	char	*arg;
// 	bool	double_quote;
// 	bool	single_quote;

// 	double_quote = false;
// 	single_quote = false;
// 	arg = malloc(ft_strlen(input) + 1);
// 	if (!arg)
// 		return (NULL);
// 	*arg_idx = 0;
// 	while (input[*i])
// 	{
// 		ft_handle_quotes(input, i, &double_quote, &single_quote);
// 		if ((input[*i] == ' ' || input[*i] == '>' || input[*i] == '<') && !double_quote && !single_quote)
// 		{
// 			(*i)++;
// 			ft_skip_spaces(input, i);
// 			while (input[*i] && input[*i] != ' ' && input[*i] != '>' && input[*i] != '<')
// 			{
// 				if (input[*i] == '\'' || input[*i] == '\"')
// 					break;
// 				(*i)++;
// 			}
// 			ft_skip_spaces(input, i);
// 			break ;
// 		}
// 		arg[(*arg_idx)++] = input[*i];
// 		(*i)++;
// 	}
// 	arg[*arg_idx] = '\0';
// 	return (arg);
// }

//      echo "cou       cou" > out dit le "   hib      OUUHHHHH"

char    *ft_allocate_and_copy(char *input, int *i, int *arg_idx)
{
	char    *arg;
	bool    double_quote;
	bool    single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(ft_strlen(input) + 1);
	if (!arg)
		return (NULL);
	*arg_idx = 0;

	while (input[*i])
	{
		ft_handle_quotes(input, i, &double_quote, &single_quote);
		if ((input[*i] == '>' || input[*i] == '<') && !double_quote && !single_quote)
		{
			break; // Stop copying when a redirection is encountered outside quotes
		}
		else if (input[*i] == ' ' && !double_quote && !single_quote)
		{
			break; // Stop copying when a space is encountered outside quotes
		}
		arg[(*arg_idx)++] = input[*i];
		(*i)++;
	}
	arg[*arg_idx] = '\0';
	ft_skip_spaces(input, i); // Skip spaces after an argument is copied

	// If redirection is encountered, skip the filename after it
	if (input[*i] == '>' || input[*i] == '<')
	{
		(*i)++; // skip the redirection character
		ft_skip_spaces(input, i); // skip spaces after redirection
		while (input[*i] && input[*i] != ' ' && input[*i] != '>' && input[*i] != '<')
		{
			if (input[*i] == '\'' || input[*i] == '\"')
				break;
			(*i)++;
		}
		ft_skip_spaces(input, i); // skip spaces after filename
	}

	return (arg);
}

/*Fonction 3: Copie d'un argument dans le tableau d'arguments*/
char	**ft_copy_argument(char *input, t_parser *parser)
{
	char	*arg;
	int		arg_idx;

	arg = ft_allocate_and_copy(input, &(parser->i), &arg_idx);
	if (!arg)
		return (NULL);
	if (arg_idx > 0)
		parser->args[(parser->idx)++] = arg;
	else
		free(arg);
	ft_skip_spaces(input, &(parser->i));
	return (parser->args);
}

/*Fonction 4 (principale): Parse Input avec Quotes*/
char	**ft_parse_input_quote(char *input)
{
	t_parser	parser;
	int			arg_count;

	parser.i = is_redir_at_beginning(input, 0);
	arg_count = count_args_single_quotes(input); //compter les doubles ?
	parser.args = malloc((arg_count + 1) * sizeof(char *)); //probleme de malloc des doubles ?
	parser.in_quote = false;
	parser.idx = 0;
	if (!parser.args)
		return (NULL);
	while (input[parser.i])
	{
		parser.args = ft_copy_argument(input, &parser);
		if (!parser.args)
			return (NULL);
	}
	parser.args[parser.idx] = NULL;
	return (parser.args);
}

void	ft_all_builtins_verif(t_command *current)
{
	int		cmd_count;
	char	**tab;
	cmd_count = 0;
	while (current)
	{
		if (ft_strncmp(current->command, "pwd", 3) == 0)
		{
			ft_builtin_pwd_fd(STDOUT_FILENO);
			exit (0);
		}
		if (ft_strncmp(current->command, "echo ", 5) == 0)
		{
			// if (current->command_path != NULL)
			// {
			// 	printf("ccmd\n");
			// 	return ;
			// }
				tab = ft_parse_input_quote(current->command);
				// int i = 0;
				// while(tab[i])
				// {
				// 	printf("%stab[%d] : %s%s\n", GREEN, i, tab[i], RESET);
				// 	i++;
				// }
				ft_builtin_echo_fd(tab);
				exit (0);
		}
		cmd_count++;
		current = current->next;
		// printf("\n");  // Just to separate different commands
	}
}
