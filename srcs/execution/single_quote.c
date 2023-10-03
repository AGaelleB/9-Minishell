/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/03 14:21:26 by abonnefo         ###   ########.fr       */
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

char **parse_input_quote(char *input)
{
	bool	in_quote;
	int		arg_count;
	int		arg_idx;
	int		idx;
	char	**args;
	char	*arg;

	arg_count = count_args(input);
	args = malloc((arg_count + 1) * sizeof(char *));
	if (!args)
		return (NULL);
	in_quote = false;
	idx = 0;
	while (*input)
	{
		arg = malloc(strlen(input) + 1);
		if (!arg)
			return (NULL);

		arg_idx = 0;
		while (*input && (in_quote || (*input != ' ' && *input != '>' && *input != '<')))
		{
			if (*input == '\'')
				in_quote = !in_quote;
			else
				arg[arg_idx++] = *input;
			input++;
		}
		if (!in_quote && (*input == '>' || *input == '<'))
			break;
		arg[arg_idx] = '\0';
		if (arg_idx > 0)
			args[idx++] = arg;
		else
			free(arg);

		while (*input == ' ')
			input++;
	}
	
	// int i = 0;
	// while(args[i])
	// {
	// 	printf("%sargs[%d] = %s%s\n", BLUE, i, args[i], RESET);
	// 	i++;
	// }	

	args[idx] = NULL;
	return (args);
}


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