/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_and_out.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:06:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/10 12:07:48 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*clean_file_name(t_token *token)
{
	// bool in_quote;
	char *file_name;
	t_token *temp;
	int i;
	int y;
	
	i = 0;
	y = 0;
	// in_quote = false;
	file_name = NULL;
	temp = token->next;
	printf("je suis rentre\n");
	// while(temp)
	// {
		// printf("1ere boucle\n");
		while(temp->split_value[i])
		{
			printf("2me boucle\n");
			if (temp->split_value[i] == '\'' || temp->split_value[i] == '"')
			{
				printf("IF ??\n");
				i++;
			}
			printf("j'associe\n");
			file_name[y] = temp->split_value[i];
			printf("jai associe\n");
			i++;
			y++;
		}
		printf("evolution file_name %s\n", file_name);
		// temp = temp->next;
	// }
	return (file_name);
}

char	*verif_file_name(t_token *token)
{
	printf("token avant %s\n", token->next->split_value);
	char *file_name;
	file_name = clean_file_name(token);
	printf("token APRES %s\n", file_name);
	if(check_valid_identifier(file_name[0]) == 1)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
		ft_putchar_fd(file_name[0], 2);
		ft_putstr_fd("\n", 2);
		exit(-1);
	}
	return (file_name);
}

int	redirect_file_out(t_command *current, t_token *token)
{
	char	*filename;

	if (current->fd_out != 1)
		close(current->fd_out);
	filename = verif_file_name(token);
	// filename = token->next->split_value;
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
	verif_file_name(token);
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
	verif_file_name(token);
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
