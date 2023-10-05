/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 16:27:55 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/05 12:01:48 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char *add_spaces_around_redirections(char *input)
{
	char	*new_input;
	int		i;
	int		j;
	int		new_len;
	bool	single_quote;

	i = 0;
	new_len = ft_strlen(input);
	single_quote = false;
	// echo 'coucou>te'st
	while(input[i])
	{
		if(input[i] == '>' || input[i] == '<')
			new_len += 2; // adding space before and after the character
		i++;
	}
	// create a new string with the calculated length
	new_input = (char *)malloc(sizeof(char) * (new_len + 1));
	if(!new_input)
	{
		perror("malloc");
		return (NULL);
	}
	// copy the original string into the new string, adding spaces as needed
	i = 0;
	j = 0;
	while(input[i])
	{
		if(input[i] == '\'')
			single_quote = !single_quote;
		if((!single_quote) && (input[i] == '>' || input[i] == '<'))
		{
			new_input[j] = ' ';
			j++;
			new_input[j] = input[i];
			j++;
			if(input[i + 1] == input[i]) // checking for ">>" or "<<"
			{
				new_input[j] = input[i + 1];
				i++; // skip the next character
				j++;
			}
			new_input[j] = ' ';
		}
		else
			new_input[j] = input[i];
		i++;
		j++;
	}
	new_input[j] = '\0';
	return (new_input);
}

t_command	*get_command(char *input, char **envp)
{
	// printf("*** get_command ***\n");
	t_command	*head;
	t_command	*current;
	t_command	*new_cmd;
	char		**command;
	int			i;

	head = NULL;
	current = NULL;
	command = split_string(input, '|');
	i = 0;

	while(command[i])
	{
		new_cmd = malloc(sizeof(t_command));
		if (!new_cmd)
		{
			perror("Failed to allocate memory for new command");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->command = NULL;
		new_cmd->command = add_spaces_around_redirections(command[i]);
		new_cmd->token_head = tokenize_input(new_cmd->command, envp);
		if (!new_cmd->command)
		{
			perror("Failed to duplicate command string");
			ft_free_tab(command);
			exit(1);
		}
		new_cmd->next = NULL;
		if (!head)
		{
			head = new_cmd;
			current = head;
		}
		else
		{
			current->next = new_cmd;
			current = new_cmd;
		}
		i++;
	}
	ft_free_tab(command);
	return (head);
}

void	ft_set_args_and_paths(t_command *current, char **envp)
{
	current->command_arg = NULL;
	current->command_path = NULL;

	// printf("%scurrent->command = %s%s\n\n", GREEN, current->command, RESET);
	current->command_arg = parse_input_quote(current->command);

//////////////////////
	// int i = 0;
	// while(current->command_arg[i])
	// {
	// 	printf("%s\ncommand_arg[%d] = %s%s", YELLOW, i, current->command_arg[i], RESET);
	// 	i++;
	// }
	// printf("\n");
/////////////////////

	// i = 0;
	// while(current->command_arg[i] == NULL)
	// 	i++;
	// printf("%s*** command_arg[%d] = %s%s ***\n", GREEN, i, current->command_arg[i], RESET);

	// printf("%s*** command_arg[0] = %s ***%s\n", GREEN, current->command_arg[0], RESET);
	// printf("%s*** command_arg[1] = %s ***%s\n", GREEN, current->command_arg[1], RESET);
	// printf("%s*** command_arg[2] = %s%s ***\n", GREEN, current->command_arg[2], RESET);
	// printf("%s*** command_arg[3] = %s%s ***\n", GREEN, current->command_arg[3], RESET);
	// printf("%s*** command_arg[4] = %s%s ***\n", GREEN, current->command_arg[4], RESET);
	// printf("%s*** command_arg[5] = %s%s ***\n", GREEN, current->command_arg[5], RESET);

	current->command_path = ft_check_paths(envp, current->command_arg[0]); // bug ici pour redir au debut
}



int	child_process(t_command *current, char **envp)
{
	if (current->command_path == NULL)
	{
		write(2, "minishell: command not found: ", 31);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, "\n", 1);
		ft_free_tab(current->command_arg);
		ft_free_current(current);
		return (127);
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1)
	{
		perror("Error");
		exit(-1);
	}
	return (0);
}
