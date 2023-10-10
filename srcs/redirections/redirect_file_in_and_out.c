/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_file_in_and_out.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:06:26 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/10 12:55:13 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_space_file_name(t_command *current)
{
	printf("jy suis ??\n");
	if(current == NULL)
		printf("current null ??\n");
	if(current->command == NULL)
		printf("  commande null ??\n");
	printf("curent :%s\n", current->command);
	return(0);
}

char	*clean_file_name(t_token *token)
{
	// bool in_quote;
	char *file_name;
	t_token *temp;
	int i;
	int j;
	int k;
	
	i = 0;
	j = 0;
	k = 0;
	// in_quote = false;
	file_name = malloc(sizeof(char*)*(1000));
	temp = token->next;
	while(temp)
	{
		while(temp->split_value[i] != '\0')
		{
			if (temp->split_value[i] == '\'' || temp->split_value[i] == '"')
				i++;
			if(k == 1)
			{	
				k = 0;
				file_name[j] = ' ';
				j++;
			}
			k = add_space_file_name(token->command);
			file_name[j] = temp->split_value[i];
			i++;
			j++;
		}
		i = 0;
		k++;
		temp = temp->next;
	}
	return (file_name);
}

char	*verif_file_name(t_token *token)
{
	printf("file name before%s\n", token->next->split_value);
	char *file_name;
	file_name = clean_file_name(token);
	printf("file name after%s\n", file_name);
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
