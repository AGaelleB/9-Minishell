/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_and_out.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:06:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/10 18:05:32 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// char    *epur_filename(t_token *token_head)
// {
//     char *file_name;
//     int i;
//     int j;
//     bool in_quote;

//     i = 0;
//     j = 0;
//     file_name = NULL;
//     in_quote = false;
//     file_name = malloc(sizeof(char) * 1000);  // Notez que c'est sizeof(char) ici, pas sizeof(char*)
//     if (!file_name)
//         return NULL;  // Gestion d'erreur en cas de malloc échoué
    
//     while(token_head->command[i] != '>')
//         i++;
//     // Je suppose que vous voulez avancer jusqu'au prochain guillemet ?
//     while(token_head->command[i] != '\'' && token_head->command[i] != '\"')
//         i++;
    
//     // Boucle principale
//     while(token_head->command[i])
//     {
//         if (token_head->command[i] == '>' || token_head->command[i] == '<')
//             break;
        
//         // basculer in_quote si on est à un caractère de guillemet
//         if (token_head->command[i] == '\'' || token_head->command[i] == '\"')
//             in_quote = !in_quote;
        
//         // on ne considère le caractère courant que si nous ne sommes pas dans une citation
//         if (!in_quote)
//         {
//             // Vous pouvez décider ici si vous souhaitez inclure ou exclure les caractères non cités
//             file_name[j] = token_head->command[i];
//             j++;
//         }
//         i++;
//     }
//     file_name[j] = '\0';  // terminer la chaîne avec un null-terminator
    
//     char *tempo;
//     tempo = malloc(sizeof(char) * 1000);  // A nouveau, c'est sizeof(char)
//     if (!tempo)
//     {
//         free(file_name); // Libérer file_name pour éviter une fuite de mémoire
//         return NULL;  // Gestion d'erreur en cas de malloc échoué
//     }
    
//     j = 0;
//     while(token_head->command[i])
//     {
//         tempo[j] = token_head->command[i];
//         i++;
//         j++;
//     }
//     tempo[j] = '\0';  // s'assurer que la chaîne est terminée
    
//     token_head->command = tempo;
//     return(file_name);
// }

char	*epur_filename(t_token *token_head)
{
	char *file_name;
	int i;
	int j;
	bool in_quote;
	bool double_quote;
	size_t cmd_len;

	i = 0;
	j = 0;
	file_name = NULL;
	in_quote = false;
	double_quote = false;
	cmd_len = ft_strlen(token_head->command);

	file_name = malloc((cmd_len + 1) * sizeof(char));
	if (!file_name)
		return (NULL);
	file_name = malloc(sizeof(char*)*(1000));
	while(token_head->command[i] != '>')
		i++;
	while((token_head->command[i] != '\'') && (token_head->command[i] != '\"'))
		i++;
	while(token_head->command[i])
	{
		if (token_head->command[i] == '>' || token_head->command[i] == '<')
			break;
		if (token_head->command[i] == '\'')
			in_quote = !in_quote;
		if (token_head->command[i] == '\"')
			double_quote = !double_quote;
		if (!in_quote && (token_head->command[i] == '\"'))
			i++;
		if (!double_quote && (token_head->command[i] == '\''))
			i++;
		if(double_quote && in_quote) //lorsque lon est plus dans des quotes alors
		{
			printf("coucou\n");
			break;
		}
		file_name[j] = token_head->command[i];
		i++;
		j++;
	}
	file_name[j] = '\0';
	char *tempo;
	tempo=malloc(sizeof(char*)*(1000));;
	j = 0;
	while(token_head->command[i])
	{
		tempo[j] = token_head->command[i];
		i++;
		j++;
	}
	token_head->command = tempo;
	return(file_name);
}

char	*verif_file_name(t_token *token, t_token *token_head)
{
	char *file_name;

	file_name = NULL;
	if(token->next->split_value[0] == '\"' || token->next->split_value[0] == '\'')
		file_name = epur_filename(token_head);
	else
	{
		file_name = token->next->split_value;
		if(check_valid_caractere_filename(file_name[0]) == 1)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			ft_putchar_fd(file_name[0], 2);
			ft_putstr_fd("\n", 2);
			exit(-1);
		}
	}
	return (file_name);
}

int	redirect_file_out(t_command *current, t_token *token, t_token *token_head)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = verif_file_name(token, token_head);
	current->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_file_in(t_command *current, t_token *token, t_token *token_head)
{
	char	*filename;

	if (current->fd_in != 0)
		close(current->fd_in);
	filename = verif_file_name(token, token_head);
	current->fd_in = open(filename, O_RDONLY);
	if (current->fd_in == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_append_file_out(t_command *current, t_token *token, t_token *token_head)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = verif_file_name(token, token_head);
	current->fd_out = open(filename, O_APPEND | O_WRONLY, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	is_redir_at_beginning(char *input, int i)
{
	while (input[i] == ' ')
		i++;
	if ((input[i] == '>') || (input[i] == '<')
		|| (input[i] == '>' && input[i + 1] == '>')
		|| (input[i] == '<' && input[i + 1] == '<'))
	{
		if ((input[i] == '>' && input[i + 1] == '>')
			|| (input[i] == '<' && input[i + 1] == '<'))
			i += 2;
		else if (input[i] == '>' || input[i] == '<')
			i++;
		while (input[i] == ' ')
			i++;
		while (input[i] != ' ')
			i++;
		return (is_redir_at_beginning(input, i));
	}
	return (i);
}
