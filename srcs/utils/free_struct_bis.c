/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct_bis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:44:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/08 16:03:02 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_export_str(t_export *export)
{
	if (export->new)
		free(export->new);
	if (export->str)
		free(export->str);
	if (export)
		free(export);
}

void	free_export_basic(t_export *export)
{
	if (export->new)
		free(export->new);
	if (export)
		free(export);
}

void	exec_free_builtins(t_process_data *data, t_env *env)
{
	ft_builtin_write_exit_process(data, env);
	ft_free_tab(data->command->command_arg_main);
	free_child(data, env);
	exit(g_exit_status);
}
