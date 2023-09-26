/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:38:56 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/26 16:13:02 by abonnefo         ###   ########.fr       */
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
t_quote	*parse_input(char *input)
{
	t_quote	*substr_list;
	char	*start;
	bool	in_single_quote;
	
	substr_list = NULL;
	start = input;
	in_single_quote = false;

	bool is_start = true;

	while (*input)
	{
		if (*input == '\'')
		{
			if (in_single_quote == true)
			{
				// end of quote
				*input = '\0';
				substr_list = add_str_to_list(substr_list, start);
				start = input + 1;
				in_single_quote = false;
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
				in_single_quote = true;
			}
		}
		else
		{
			if (ft_isspace(*input))
			{
				if (!is_start)
				{ // Si l'espace n'est pas au début, on le traite comme un séparateur
					*input = '\0';
					substr_list = add_str_to_list(substr_list, start);
					start = input + 1;
					is_start = true;
				}
			}
			else
				is_start = false;
		}
		input++;
	}
	if (!is_start || in_single_quote)
		substr_list = add_str_to_list(substr_list, start);
	
	return (substr_list);
}

void ft_cat_list_quote(t_quote *substr_list)
{
	char	*command;
	char	*temp;
	
	command = NULL;
	temp = NULL;
	while (substr_list)
	{
		temp = concat_strings(command, substr_list->str);
		free(command);
		command = temp;
		printf("%ssubstr_list->str : %s%s\n", MAGENTA, substr_list->str, RESET); // PRINT
		printf("%scommand : %s%s\n\n", GREEN, command, RESET); // PRINT
		substr_list = substr_list->next;
	}
	// use command - get command ?
	free(command);
}
