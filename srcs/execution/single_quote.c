/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/28 16:50:53 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


t_quote	*add_str_to_list(t_quote *list, char *str)
{
	t_quote	*new_list_str;
	t_quote	*temp;
	
	new_list_str = malloc(sizeof(t_quote));
	if (!new_list_str)
		exit(1);
	
	new_list_str->str = str;
	new_list_str->next = NULL;
	if (!list)
		return (new_list_str);
	
	temp = list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_list_str;

	// printf("%snew_list_str->str: %s%s\n", CYAN, new_list_str->str, RESET); // PRINT

	return (list);
}

t_quote	*parse_input_quote(char *input, char **envp)
{
	t_quote	*substr_list;
	char	*start;
	bool	flag_open_quote;
	
	substr_list = NULL;
	start = input;
	flag_open_quote = false;
	while (*input)
	{
		if (*input == '\'')
		{
			if (flag_open_quote == true)
			{
				*input = '\0';
				substr_list = add_str_to_list(substr_list, start);
				start = input + 1;
				flag_open_quote = false;
			}
			else
			{
				if (start != input)
				{
					*input = '\0';
					substr_list = add_str_to_list(substr_list, start);
				}
				start = input + 1;
				flag_open_quote = true;
			}
		}
		input++;
	}
	if (start != input)
		substr_list = add_str_to_list(substr_list, start);
	ft_cat_list_quote(substr_list, envp);
	return (substr_list);
}

void ft_cat_list_quote(t_quote *substr_list, char **envp)
{
	t_command	*new_commands;
	char		*command;
	char		*temp;

	new_commands = NULL;
	command = NULL;
	temp = NULL;
	
	int i = 0;
	while (substr_list)
	{
		temp = ft_strjoin(command, substr_list->str);
		free(command);
		command = temp;

		printf("%ssubstr_list->str[%d] : %s%s\n", MAGENTA, i, substr_list->str, RESET); // PRINT
		// printf("%scommand : %s%s\n", MAGENTA, command, RESET); // PRINT
		substr_list = substr_list->next;
		// i++;
	}
	new_commands = get_command(command);
	count_and_set_pipes(command, new_commands);
	
	ft_all_builtins_verif(new_commands);

	print_commands_and_tokens(new_commands);
	if(new_commands != NULL)
		execve_fd(new_commands, envp);

	free(command);
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