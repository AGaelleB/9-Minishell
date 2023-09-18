/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfresque <bfresque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/09/18 14:33:26 by bfresque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
si < prendre argc passé avant comme infile fd[0]
si > prendre argc passé apres comme outfile fd[1]
*/

int	redirect_file_out(t_command **cmd, t_token *cur, t_e_token_type type)
{
	char	*filename;

	if (type == TYPE_REDIR_OUT)
	{
		if ((*cmd)->fd_out != 1)
			close((*cmd)->fd_out);
		filename = cur->next->split_value;
		printf("%s\n", filename); //printf
		(*cmd)->fd_out = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if ((*cmd)->fd_out == -1)
		{
			perror(filename);
			return (-1);
		}
	}
	return (0);
}

int	open_fd(t_command **cmdl)
{
	t_token	*cur;

	cur = (*cmdl)->token;
	// printf("ici\n");
	while (cur)
	{
		// if (cur->type == FILE_IN || cur->type == LIMITOR)
		// 	if (redirect_file_in(cmdl, cur, cur->type) == -1)
		// 		return (-1);
		// printf("boucle\n");
		// printf("cur->type : %d\n", cur->type);
		if (cur->type == TYPE_REDIR_OUT)
		{
			// printf("coucou on me voit ??\n");
			if (redirect_file_out(cmdl, cur, cur->type) == -1)
				return (-1);
		}
		cur = cur->next;
	}
	return (0);
}


void execve_fd(t_command *current, char **envp)
{
	t_command	*command;
	// t_token		*token;	//NEW
	pid_t		pid;
	pid_t		*child_pids;
	int			infile;
	int			num_children;
	int			index;
	int			i;

	// if (current->token == NULL)	//NEW
	// {
	// 	printf("TOKEN POINT SUR NULL !!!!!\n");
	// 	return;
	// }
	// token = current->token;	//NEW
	command = current;
	infile = 0;
	num_children = 0;
	index = 0;
	i = -1;
	while (current)
	{
		num_children++;
		current = current->next;
	}
	current = command;
	child_pids = malloc(num_children * sizeof(pid_t));
	if (!child_pids)
		exit(1);
	open_fd(&current);
	while (current)
	{
		// printf("juste AVANT---------\n");	//NEW
		// if (ft_strcmp_minishell(current->command, ">") == 0)
		// if (token->type == TYPE_REDIR_OUT)
		// if (current->token->type == TYPE_REDIR_OUT)
		// {
		// 	printf("ici DANS verif redir\n");
		// 	if (redirect_file_out(&current, current->token->next, TYPE_REDIR_OUT) == -1)
		// 	{
		// 		perror("Redirection de sortie a échoué");
		// 		exit(1);
		// 	}
		// }
		// printf("juste APRES++++++++++\n");	//NEW
		if (pipe(current->fd) == -1)
		{
			perror("pipe");
			free(child_pids);
			exit(1);
		}
		pid = fork();
		child_pids[index++] = pid;
		if (pid == 0) // Child
		{
			close(current->fd[0]);
			dup2(infile, 0);
			if (current->next)
				dup2(current->fd[1], 1);
			close(current->fd[1]);
			close_fd();
			if(child_process(current, envp) == 127)
			{
				free(child_pids);
				exit(127);
			}
		}
		else if (pid > 0) // Parent
		{
			close(current->fd[1]);
			if (infile != 0)
				close(infile);
			infile = current->fd[0];
		}
		else
		{
			perror("fork");
			free(child_pids);
			exit(1);
		}
		current = current->next;
	}
	signal(SIGINT, SIG_IGN);
	while (i < command->nb_pipes)
	{
		++i;
		waitpid(child_pids[i], NULL, 0);
	}
	signal(SIGINT, ft_signal_ctrl_C);
	free(child_pids);
	if (infile != 0)
		close(infile);
}



// void execve_fd(t_command *current, char **envp)
// {
// 	t_command *command;
// 	pid_t pid;
// 	pid_t *child_pids;
// 	int infile;
// 	int num_children;
// 	int index;
// 	int i;

// 	command = current;
// 	infile = 0;
// 	num_children = 0;
// 	index = 0;
// 	i = -1;
// 	while (current)
// 	{
// 		num_children++;
// 		current = current->next;
// 	}
// 	current = command;
// 	child_pids = malloc(num_children * sizeof(pid_t));
// 	if (!child_pids)
// 		exit(1);
	
// 	t_command *temp = command; // Utilisation d'une variable temporaire
	
// 	while (temp)
// 	{
// 		// if (temp->token == NULL)
// 		// {
// 		// 	printf("TOKEN POINT SUR NULL !!!!!\n");
// 		// 	return;
// 		// }
// 		// printf("type actuel : %d\n", temp->token->type);
// 		// printf("ici AVANT verif redir\n");
// 		if (temp->token->type == TYPE_REDIR_OUT)
// 		{
// 			printf("ici DANS verif redir\n");
// 			if (redirect_file_out(&temp, temp->token->next, TYPE_REDIR_OUT) == -1)
// 			{
// 				perror("Redirection de sortie a échoué");
// 				exit(1);
// 			}
// 		}
// 		// printf("ici APRES verif redir\n");
// 		if (pipe(temp->fd) == -1)
// 		{
// 			perror("pipe");
// 			free(child_pids);
// 			exit(1);
// 		}
		
// 		pid = fork();
// 		child_pids[index++] = pid;
// 		if (pid == 0)
// 		{
// 			if (temp->fd[0] != -1)
// 			{
// 				dup2(temp->fd[0], 0);
// 				close(temp->fd[0]);
// 			}
// 			if (temp->fd[1] != -1)
// 			{
// 				dup2(temp->fd[1], 1);
// 				close(temp->fd[1]);
// 			}

// 			if (temp->token->type != TYPE_REDIR_OUT)
// 				ft_set_args_and_paths(temp, envp);
// 			if (temp->command_path == NULL)
// 			{
// 				write(2, "minishell: command not found: ", 31);
// 				write(2, temp->command_arg[0], ft_strlen(temp->command_arg[0]));
// 				write(2, "\n", 1);
// 				ft_free_tab(temp->command_arg);
// 				ft_free_current(temp);
// 				exit(127);
// 			}
// 			else if (execve(temp->command_path, temp->command_arg, envp) == -1)
// 			{
// 				perror("Error");
// 				exit(-1);
// 			}
// 		}
// 		else if (pid > 0)
// 		{
// 			close(temp->fd[1]);
// 			if (infile != 0)
// 				close(infile);
// 			infile = temp->fd[0];
// 		}
// 		else
// 		{
// 			perror("fork");
// 			free(child_pids);
// 			exit(1);
// 		}
// 		temp = temp->next;
// 	}

// 	while (i < command->nb_pipes)
// 	{
// 		++i;
// 		waitpid(child_pids[i], NULL, 0);
// 	}

// 	free(child_pids);
// 	if (infile != 0)
// 		close(infile);
// }



// RAYAN
// 	int status;
// 	int i = 0;
// 	printf("nombre de pipes final : %d\n", command->nb_pipes);
// 	while (i < command->nb_pipes)
// 	{
// 		// printf("test\n");
// 		waitpid(0, &status, -1);
// 		i++;
// 	}
// 	WEXITSTATUS(status);