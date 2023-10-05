/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 12:03:50 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int count_args(char *input)
{
	int		count;
	bool	in_quote;
	bool	was_space; // pour compter le premier mot même s'il n'y a pas d'espace devant
	
	count = 0;
	in_quote = false;
	was_space = true;
	while (*input)
	{
		if (*input == '\'')
			in_quote = !in_quote;
		if (!in_quote && *input == ' ')
			was_space = true;
		else if (was_space)
		{
			count++;
			was_space = false;
		}
		input++;
	}
	return (count);
}

int	is_redir_at_beginning(char *input, int i)
{

	while (input[i] == ' ')
		i++;

	if (input[i] == '>' || input[i] == '<' || (input[i] == '>' && input[i + 1] == '>'))
	{
		if (input[i] == '>' || input[i] == '<')
			i++;
		else if (input[i] == '>' && input[i + 1] == '>')
			i += 2;
		while (input[i] == ' ')
			i++;
		while (input[i] != ' ')
			i++;
		return (is_redir_at_beginning(input, i));
	}
	return (i);
}

char **parse_input_quote(char *input)
{
	bool	in_quote;
	int		arg_count;
	int		arg_idx;
	int		idx;
	char	**args;
	char	*arg;
	int		i;

	i = 0;
	i = is_redir_at_beginning(input, i);
	arg_count = count_args(input);
	args = malloc((arg_count + 1) * sizeof(char *));
	in_quote = false;
	idx = 0;
	
	if (!args)
		return (NULL);
	while (input[i])
	{
		arg = malloc(ft_strlen(input) + 1);
		if (!arg)
			return (NULL);
		arg_idx = 0;
		while (input[i] && (in_quote || (input[i] != ' ' && input[i] != '>' && input[i] != '<')))
		{
			if (input[i] == '\'')
				in_quote = !in_quote;
			else
				arg[arg_idx++] = input[i];
			i++;
		}
		if ((!in_quote) && (input[i] == '>' || input[i] == '<'))
			break;
		arg[arg_idx] = '\0';
		if (arg_idx > 0)
			args[idx++] = arg;
		else
			free(arg);
		while (input[i] == ' ')
			i++;
	}
	args[idx] = NULL;
	return (args);
}




// char **parse_input_quote(char *input)
// {
// 	bool	in_quote;
// 	int		arg_count;
// 	int		arg_idx;
// 	int		idx;
// 	char	**args;
// 	char	*arg;
// 	int		i;

// 	i = 0;
// 	arg_count = count_args(input);
// 	args = malloc((arg_count + 1) * sizeof(char *));
// 	in_quote = false;
// 	idx = 0;

// 	if (!args)
// 		return (NULL);

// 	is_redir_at_beginning(input);

// 	// while (input[i] == ' ')
// 	// 	i++;

// 	// while (input[i] == '>' || input[i] == '<' || (input[i] == '>' && input[i + 1] == '>'))
// 	// {
// 	// 	while (input[i] == ' ')
// 	// 		i++;
// 	// 	if (input[i] == '>' || input[i] == '<')
// 	// 		i++;
// 	// 	else
// 	// 		i += 2;
// 	// 	while (input[i] == ' ')
// 	// 		i++;
// 	// 	while (input[i] != ' ')
// 	// 		i++;
// 	// 	if (input[i + 1] == '>')
// 	// 		i++;
// 	// }

// 	while (input[i])
// 	{
// 		arg = malloc(ft_strlen(input) + 1);
// 		if (!arg)
// 			return (NULL);
// 		arg_idx = 0;

// 		// Ajoutez cette condition pour exclure les arguments après une redirection
// 		if (input[i] == '>' || input[i] == '<')
// 		{
// 			while (input[i] != ' ')
// 				i++;
// 			continue;
// 		}

// 		while (input[i] && (in_quote || (input[i] != ' ' && input[i] != '>' && input[i] != '<')))
// 		{
// 			if (input[i] == '\'')
// 				in_quote = !in_quote;
// 			else
// 				arg[arg_idx++] = input[i];
// 			i++;
// 		}
// 		if ((!in_quote) && (input[i] == '>' || input[i] == '<'))
// 			break;
// 		arg[arg_idx] = '\0';
// 		if (arg_idx > 0)
// 			args[idx++] = arg;
// 		else
// 			free(arg);

// 		while (input[i] == ' ')
// 			i++;
// 	}

// 	args[idx] = NULL;
// 	return (args);
// }












////////////////// VERSION AVANT input[i] ///////////////////

// char **parse_input_quote(char *input)
// {
// 	bool	in_quote;
// 	int		arg_count;
// 	int		arg_idx;
// 	int		idx;
// 	char	**args;
// 	char	*arg;
// 	int		i;
// 	int		start_with_redirection; //

// 	char	*str = input;
// 	int		compteur = 0;

// 	i = 0;
// 	start_with_redirection = 0;
// 	arg_count = count_args(input);
// 	args = malloc((arg_count + 1) * sizeof(char *));
// 	if (!args)
// 		return (NULL);
// 	in_quote = false;
// 	idx = 0;
// 	int tour = 1;


// 	// while(str[compteur] == ' ')
// 	// 	compteur++;

// 	// if (str[compteur] == '>') // if the first arg is a redirection ">" "<" "<<" ">>"
// 	// {
// 	// 	printf("t'as compris que je commencais avec un '>' ???\n");
// 	// 	printf("str[%d] = %c\n", compteur, str[compteur]);
// 	// 	start_with_redirection = 1;
// 	// 	// printf("start_with_redirection = %d - tour n.%d\n", start_with_redirection, tour); ////////////
// 	// }

// 	while(input[i] == ' ')
// 		i++;

// 	if (input[i] == '>') // if the first arg is a redirection ">" "<" "<<" ">>"
// 	{
// 		printf("t'as compris que je commencais avec un '>' ???\n");
// 		printf("input[%d] = %c\n", i, str[i]);
// 		start_with_redirection = 1;
// 		// printf("start_with_redirection = %d - tour n.%d\n", start_with_redirection, tour); ////////////
// 	}
	

// 	while (*input)
// 	{
// 		// if (*input == '>') // if the first arg is a redirection ">" "<" "<<" ">>"
// 		// {
// 		// 	printf("t'as compris que je commencais avec un '>' ???\n");
// 		// 	start_with_redirection++;
// 		// 	printf("start_with_redirection = %d - tour n.%d\n", start_with_redirection, tour); ////////////
// 		// }
		
// 		// printf("input = %s - tour n.%d\n", input, tour); ////////////
// 		arg = malloc(ft_strlen(input) + 1);
// 		if (!arg)
// 			return (NULL);
// 		arg_idx = 0;

// 		while (*input && (in_quote || (*input != ' ' && *input != '>' && *input != '<')))
// 		{
// 			if (*input == '\'')
// 				in_quote = !in_quote;
// 			else
// 				arg[arg_idx++] = *input;
// 			input++;
// 		}

// 		// if ((!in_quote) && (*input == '>' || *input == '<'))
// 		// if (start_with_redirection == arg_count)
// 		// if ((start_with_redirection == arg_count) && (!in_quote) && (*input == '>' || *input == '<'))
// 		if ((start_with_redirection != 1) && (!in_quote) && (*input == '>' || *input == '<'))
// 		{
// 			printf("%sBREAK start_with_redirection = %d - tour n.%d%s\n", RED, start_with_redirection, tour, RESET);
// 			break;
// 		}
// 		arg[arg_idx] = '\0';
// 		if (arg_idx > 0)
// 			args[idx++] = arg;
// 		else
// 			free(arg);

// 		while (*input == ' ')
// 			input++;
		
// 		printf("FIN start_with_redirection = %d - tour n.%d\n\n", start_with_redirection, tour);
// 		tour++;
// 		start_with_redirection++;
// 		i++;

// 	}
// 	args[idx] = NULL;
// 	return (args);
// }




///////// VERSION HIER SOIR ///////////////////

// char **parse_input_quote(char *input)
// {
// 	bool	in_quote;
// 	int		arg_count;
// 	int		arg_idx;
// 	int		idx;
// 	char	**args;
// 	char	*arg;

// 	arg_count = count_args(input);
// 	args = malloc((arg_count + 1) * sizeof(char *));
// 	if (!args)
// 		return (NULL);
// 	in_quote = false;
// 	idx = 0;
// 	while (*input)
// 	{
// 		arg = malloc(strlen(input) + 1);
// 		if (!arg)
// 			return (NULL);

// 		arg_idx = 0;
// 		while (*input && (in_quote || (*input != ' ' && *input != '>' && *input != '<')))
// 		{
// 			if (*input == '\'')
// 				in_quote = !in_quote;
// 			else
// 				arg[arg_idx++] = *input;
// 			input++;
// 		}
// 		if (!in_quote && (*input == '>' || *input == '<')) // CHANTIER IS HERE - BOOL
// 			break;
// 		arg[arg_idx] = '\0';
// 		if (arg_idx > 0)
// 			args[idx++] = arg;
// 		else
// 			free(arg);

// 		while (*input == ' ')
// 			input++;
// 	}
// 	printf("%s*** args[0] = %s%s ***\n", YELLOW, args[0], RESET);
// 	printf("%s*** args[1] = %s%s ***\n", YELLOW, args[1], RESET);

// 	args[idx] = NULL;
// 	return (args);
// }







/*
TO DO DE LUNDI:

1- corriger les problemes d espaces qui fait que les commandes sont collés genre :
cat celine.txt | rev>out

gerer la redir en debut de cmd :
> coucou echo test
on a rien

3- verifier les free valgrind etc
*/


/*
cassé chez Rayan : 
ec'h'o     t'rst'
	-> on doit avoir "trst"
ec'h'o'   ' t'rst'
	-> on doit avoir "echo   : command not found"

echo -n -a -nnn -er -nnnnnn -nae -nnn  bonjour
	-> -a -nnn -er -nnnnnn -nae -nnn  bonjourminishell$> 
*/