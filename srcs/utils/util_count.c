/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:05:15 by abonnefo          #+#    #+#             */
/*   Updated: 2023/11/20 16:53:50 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipe(char *input)
{
	int	count_pipe;
	int	i;

	count_pipe = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
			count_pipe++;
		i++;
	}
	return (count_pipe);
}

void	verif_nb_args_exit(void)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	g_exit_status = 1;
	return ;
}

int	verif_nb_pipe_exit(char **args)
{
	char	*str;

	str = epurstr(args[0]);
	g_exit_status = validate_exit_status_process(str);
	free(str);
	return (0);
}
