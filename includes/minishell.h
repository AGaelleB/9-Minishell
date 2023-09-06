/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 14:11:23 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/06 14:20:36 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <stdint.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../includes/ft_printf.h"
# include "../includes/get_next_line.h"
# include "../includes/libft.h"

typedef struct s_cmd
{
	char	*path;
	char	**args;
	
}	t_cmd;


/********************************* main.c ************************************/

int		ft_strchr_slash(char *str, char c);
int		ft_strncmp_minishell(char *s1, char *s2, int n);
char	*ft_strjoin_minishell(char *s1, char *s2);

void	ft_print_error(char *str);
void	ft_free_tab(char **tab);
char	*ft_check_paths(char **envp, char *args);
char	**split_string(const char *str, char delimiter);

#endif