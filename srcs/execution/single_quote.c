/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/29 16:15:42 by abonnefo         ###   ########.fr       */
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

char	**parse_input_quote(char *input)
{
	int		arg_count;
	bool	in_quote;
	char	**args;
	int		idx;
	char	*arg;
	int		arg_idx;

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
		while (*input && (in_quote || *input != ' '))
		{
			if (*input == '\'')
				in_quote = !in_quote;
			else
				arg[arg_idx++] = *input;
			input++;
		}
		arg[arg_idx] = '\0';
		if (arg_idx > 0)
			args[idx++] = arg;
		else
			free(arg);
		while (*input == ' ')
			input++; // Skip spaces
	}
	args[idx] = NULL;
	return (args);
}

/*
On stock correctement avec nos espaces les valeurs dans substr_list->str
on voudrait l utiliser pour les execution mais on y arrive pas
est-ce qu on est obligé d'utiliser des double ** ?
est-ce que ca va nous poser probleme l utilisation de notre split ? 
	on ne peut pas split sur les simple quote car on les retire 
*/


/*
On execute et ne doit pas s executer :
echo' test'
echo' 'test

On reagit mal :
echo test ls
	-> on a tout de collé
echo ' 'ls
	-> on devrait afficher l'espace devant le "ls"
echo 'debut              fin'       test
	-> on doit avoir "debut              fin test"
minishell$> echo 1 2 3 4 5
	-> on doit avoir "1 2 3 4 5




cassé chez Rayan : 
ec'h'o     t'rst'
	-> on doit avoir "trst"
ec'h'o'   ' t'rst'
	-> on doit avoir "echo   : command not found"




shell:
bash-5.1$ ec'h'o' ' t'rst'
bash: echo : command not found
bash-5.1$ ec'h'o ' ' t'rst'
  trst
bash-5.1$ ec'h'o ' 't'rst'
 trst
bash-5.1$ ec'h'o' 't'rst'
bash: echo trst: command not found

*/