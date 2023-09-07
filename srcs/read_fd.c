/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/07 15:25:20 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirect_file_in(t_command **command_arg, t_token *token, t_e_token_type e_tokentype)
{
	
	return (0);
}

int	redirect_file_out(t_command **command_arg, t_token *token, t_e_token_type e_tokentype)
{
	
	return (0);
}

int	open_fd(t_command *current)
{
	current = current->command_arg[0];
	while (current)
	{

		current = current->next;
	}
	return (0);
}
