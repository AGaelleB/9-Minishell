/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/27 14:50:48 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//2. Crée une Fonction pour Ajouter des Sous-chaînes à ta Liste
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

	return (list);
}

//3. Crée une Fonction pour Parcourir l'Entrée et Construire la Liste de Sous-chaînes
t_quote	*parse_input_quote(char *input, char **envp)
{
	t_quote	*substr_list;
	char	*start;
	bool	flag_open_quote;
	bool	flag_separator = true;
	
	substr_list = NULL;
	start = input;
	flag_open_quote = false;
	while (*input)
	{
		if (*input == '\'')
		{
			if (flag_open_quote == true)
			{
				// end of quote
				*input = '\0';
				substr_list = add_str_to_list(substr_list, start);
				start = input + 1;
				flag_open_quote = false;
			}
			else
			{
				if (start != input)
				{
					// if there are characters before the quote, add them to the list
					*input = '\0';
					substr_list = add_str_to_list(substr_list, start);
				}
				start = input + 1;
				flag_open_quote = true;
			}
		}
		else if (!flag_open_quote && ft_isspace(*input)) 
		{
			if (!flag_separator)
			{ // Si l'espace n'est pas au début, on le traite comme un séparateur
				*input = '\0';
				substr_list = add_str_to_list(substr_list, start);
				start = input + 1;
				flag_separator = true;
			}
			else
				flag_separator = false;
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
	while (substr_list)
	{
		temp = ft_strjoin(command, substr_list->str);
		free(command);
		command = temp;
		printf("%ssubstr_list->str : %s%s\n", MAGENTA, substr_list->str, RESET); // PRINT
		printf("%scommand : %s%s\n\n", GREEN, command, RESET); // PRINT
		substr_list = substr_list->next;
	}
	new_commands = get_command(command);
	count_and_set_pipes(command, new_commands);
	if(new_commands != NULL)
		execve_fd(new_commands, envp);
	free(command);
}


/*
On execute et ne doit pas s executer :
echo' test'
echo' 'test


On reagit mal :
echo test ls
	-> on a tout de collé


*/