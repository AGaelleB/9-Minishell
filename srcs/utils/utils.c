/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:32:38 by bfresque          #+#    #+#             */
/*   Updated: 2023/10/06 17:49:11 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	count_and_set_pipes(char *input, t_command *command)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			count++;
		i++;
	}
	command->nb_pipes = count;
}

void	ft_close_fd(void)
{
	int	fd;

	fd = 3;
	while (fd < 100)
	{
		close(fd);
		fd++;
	}
}

int	is_empty_or_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isspace((unsigned char)str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_isspace(int c)
{
	if (c == ' ')
		return (1);
	return (0);
}

/////////////////////////////////////////////////////////////////////////
void print_commands_and_tokens(t_command *head)
{
	t_command *current_cmd = head;
	t_token *current_token;
	int cmd_count = 0;

	while (current_cmd)
	{
		printf("%sCommand %d: %s%s\n", MAGENTA, cmd_count, current_cmd->command, RESET);
		
		current_token = current_cmd->token_head;
		while (current_token)
		{
			printf("\t%scommand_arg: %s, type: %d\n%s", CYAN, current_token->split_value, current_token->type, RESET);
			current_token = current_token->next;
		}

		cmd_count++;
		current_cmd = current_cmd->next;
		printf("\n");  // Just to separate different commands
	}
}
