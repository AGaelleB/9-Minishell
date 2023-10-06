/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_and_out.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:06:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/06 11:55:40 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
char *extract_filename(const char *input)
{
    char *filename;
    int i = 0, j = 0;

    filename = malloc(strlen(input) + 1); // +1 for null-terminator


    while (input[i] != '\0')
    {
        if (input[i] != '\'' && input[i] != '\"')
        {
            filename[j++] = input[i];
        }
        i++;
    }
    filename[j] = '\0';

    return filename;
}

char *extract_filename(const char *input)
{
    // Buffer to hold the extracted filename.
    char *filename = NULL;

    // Check if the input starts with a quote.
    bool is_quoted = input[0] == '\'' || input[0] == '"';
    if (is_quoted)
    {
        // Find the closing quote.
        const char *end_quote = strchr(input + 1, input[0]);
        if (end_quote)
        {
            // Allocate memory and copy the filename without quotes.
            size_t len = end_quote - input - 1;
            filename = malloc(len + 1);
            if (filename)
            {
                strncpy(filename, input + 1, len);
                filename[len] = '\0';
            }
        }
    }
    else
    {
        // If not quoted, simply duplicate the input as the filename.
        filename = strdup(input);
    }

    return filename;
}

int redirect_file_out(t_command *current, t_token *token) // >
{
    char *filename;
    char *extracted_filename;

    if (current->fd_out != 1)
        close(current->fd_out);
    filename = token->next->split_value;
	// printf("")
    // Extract the filename without quotes and unnecessary spaces
    extracted_filename = extract_filename(filename);

    // Open file with the extracted filename
    current->fd_out = open(extracted_filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    // Free the extracted filename after it is used
    free(extracted_filename);

    if (current->fd_out == -1)
    {
        write(1, "minishell: ", 12);
        perror(filename);
        exit(-1);
    }
    return (0);
}
*/

int	redirect_file_out(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = token->next->split_value;
	current->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (current->fd_out == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_file_in(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_in != 0)
		close(current->fd_in);
	filename = token->next->split_value;
	current->fd_in = open(filename, O_RDONLY);
	if (current->fd_in == -1)
	{
		write(1, "minishell: ", 12);
		perror(filename);
		exit(-1);
	}
	return (0);
}

int	redirect_append_file_out(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = token->next->split_value;
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
