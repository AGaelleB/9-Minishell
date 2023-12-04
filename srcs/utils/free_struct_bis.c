/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct_bis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 16:44:57 by abonnefo          #+#    #+#             */
/*   Updated: 2023/12/04 16:45:27 by abonnefo         ###   ########.fr       */
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
