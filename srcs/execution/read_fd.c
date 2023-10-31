/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 12:06:07 by abonnefo          #+#    #+#             */
/*   Updated: 2023/10/31 10:22:43 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	handle_child_process(t_process_data *data, t_env *env)
// {
// 	// close(data->current->fd_in);
// 	// dup2(data->infile, 0);
// 	// if (data->current->next)
// 	// 	dup2(data->current->fd_out, 1);
// 	// close(data->current->fd_out);
// 	// ft_close_fd();

//     close(data->current->fd_in);
    
//     // Utilisez heredoc_fd comme entrée si défini (i.e. infile est heredoc_fd[0])
//     if (data->infile != 0)
//     {
//         dup2(data->infile, STDIN_FILENO);
//     }
//     else
//     {
//         dup2(data->current->fd_in, STDIN_FILENO);
//     }
    
//     if (data->current->next)
//     {
//         dup2(data->current->fd_out, STDOUT_FILENO);
//     }
    
//     close(data->current->fd_out);
//     ft_close_fd();
	
// 	if (data->current->fd_in != -1)
// 	{
// 		dup2(data->current->fd_in, 0);
// 		close(data->current->fd_in);
// 	}
// 	if (data->current->fd_out != -1)
// 	{
// 		dup2(data->current->fd_out, 1);
// 		close(data->current->fd_out);
// 	}
// 	// close(heredoc_fd[0]);
// 	// dup2(heredoc_fd[1], STDOUT_FILENO);
// 	// close(heredoc_fd[1]);
// 	open_fd(data->current);
// 	if (builtins_verif(data->current, env) == 1)
// 		exit(0);
// 	// ft_set_args_and_paths(data->current, env);
// 	// print_commands_and_tokens(current); // PRINT
// 	if (execve_process(data->current, env) == 127)
// 		exit(127);
// }

void	handle_child_process(t_process_data *data, t_env *env)
{
    // Si infile est défini (i.e. pour heredoc), utilisez-le comme entrée
    if (data->infile != 0)
    {
        dup2(data->infile, STDIN_FILENO);
        close(data->infile);
    }
    else if (data->current->fd_in != -1)
    {
		dup2(data->current->fd_in, STDIN_FILENO);
		close(data->current->fd_in);
    }
    
    if (data->current->fd_out != -1)
    {
		dup2(data->current->fd_out, STDOUT_FILENO);
		close(data->current->fd_out);
	}

    ft_close_fd();
	open_fd(data->current);

	if (builtins_verif(data->current, env) == 1)
		exit(0);

	if (execve_process(data->current, env) == 127)
		exit(127);
}



void	handle_parent_process(t_process_data *data)
{
	close(data->current->fd_out);
	if (data->infile != 0)
		close(data->infile);
	data->infile = data->current->fd_in;
}

void	handle_all_process(t_process_data *data, t_env *env)
{
	if (data->pid == 0)
		handle_child_process(data, env);
	else if (data->pid > 0)
		handle_parent_process(data);
	else
		exit_with_error("fork", data->child_pids);
}

void	wait_for_children(t_command *command, pid_t *child_pids)
{
	int	i;

	i = -1;
	// i = 0;
	printf("3bis - sig\n");
	signal(SIGINT, SIG_IGN);
	printf("3bis - sig after \n");
	while (i < command->nb_pipes)
	{
		++i;
		printf("i = %d\n", i);
		waitpid(child_pids[i], NULL, 0);
		// i++;
	}
	printf("3bis - olala sig\n");
	signal(SIGINT, ft_builtin_ctrl_c);
	printf("3bis - FIN sig\n");
}


// pid_t heredoc_open_fd_pipe(t_command *command, t_token **token)
// {
// 	pid_t pid = -1;

// 	if (*token && (*token)->type == TYPE_HEREDOC)
// 	{

// 		int heredoc_fd[2];
// 		if (pipe(heredoc_fd) == -1)
// 		{
// 			perror("pipe");
// 			exit(EXIT_FAILURE);
// 		}
// 		// Ici, nous simulons la création d'un processus pour gérer le heredoc.
// 		pid = fork();
// 		if (pid == 0) // C'est le processus enfant
// 		{
// 			*token = handle_multiple_heredocs(command, *token);

// 			if (command->fd_in != -1)
// 			{
// 				dup2(command->fd_in, 0);
// 				close(command->fd_in);
// 			}
			
// 			// Code du processus enfant pour le heredoc...
// 			// printf("Je termine le processus enfant après avoir terminé le heredoc\n");
// 			exit(0); // Terminez le processus enfant après avoir terminé le heredoc.
// 		}
// 		else if (pid < 0) 
// 		{
// 			perror("fork");
// 			exit(EXIT_FAILURE); // Gestion d'erreur pour fork
// 		}
// 	}
// 	return (pid); // Renvoie l'ID du processus du heredoc, ou -1 si aucun processus n'a été créé.
// }

pid_t heredoc_open_fd_pipe(t_command *command, t_token **token, int heredoc_fd[2])
{
	pid_t pid = -1;

	if (*token && (*token)->type == TYPE_HEREDOC)
	{
		if (pipe(heredoc_fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		pid = fork();
		if (pid == 0) 
		{
			close(heredoc_fd[0]);
			dup2(heredoc_fd[1], STDOUT_FILENO);
			close(heredoc_fd[1]);
			
			*token = handle_multiple_heredocs(command, *token);

			if (command->fd_in != -1)
			{
				dup2(command->fd_in, 0);
				close(command->fd_in);
			}
			exit(0);
		}
		else if (pid < 0) 
		{
			close(heredoc_fd[0]);
			close(heredoc_fd[1]);
			perror("fork");
			exit(EXIT_FAILURE);
		}

        // Ne fermez PAS heredoc_fd[0] ici. 
        // Laissez-le ouvert pour que la commande qui utilise le heredoc comme entrée puisse le lire.
		// close(heredoc_fd[0]);  // Commentez cette ligne ou supprimez-la
		close(heredoc_fd[1]);  // Vous n'écrivez pas dans ce pipe dans le processus parent, donc fermez-le.
	}
	return pid; 
}



void execve_fd(t_command *current, t_env *env)
{
	t_process_data data;
	t_token *token;
	// pid_t heredoc_pid = -1;  // Add this line

	// current->heredoc_pid = -1;

	data.command = current;
	data.current = current;
	data.infile = 0;
	data.index = 0;
	token = current->token_head;
	init_execve(current, &(data.child_pids));

	int flag = 0;
	if (data.current->nb_pipes != 0)
	{
		while(data.current && flag == 0)
		{
			while(token && flag == 0)
			{
				// if (token->type == TYPE_HEREDOC)
				// {
					
				// 	pid_t heredoc_pid = heredoc_open_fd_pipe(current, &token, data.heredoc_fd);
				// 	waitpid(heredoc_pid, NULL, 0);
					
				// 	// heredoc_pid = heredoc_open_fd_pipe(current, &token);
				// 	flag = 1;
				// 	break;
				// }
				if (token->type == TYPE_HEREDOC)
				{
					pid_t heredoc_pid = heredoc_open_fd_pipe(current, &token, data.heredoc_fd);
					waitpid(heredoc_pid, NULL, 0);
					data.infile = data.heredoc_fd[0];  // Ici, utilisez heredoc_fd comme entrée pour la commande
					flag = 1;
					break;
				}
				token = token->next;
			}
			if (flag == 1)
				break;
			
			data.current = data.current->next;
		}
	}
	// Wait for heredoc to complete
	// if (heredoc_pid != -1)
	// {
	// 	waitpid(heredoc_pid, NULL, 0);
	// 	// cleanup(&heredoc_pid, data.infile);
	// 	// data.infile = 0;
	// 	// free(heredoc_pid);
	// }
	printf("1 - j'ai fini de wait le pid de heredoc\n");
	data.current = current;
	while (data.current)
	{
		if (pipe(data.current->fd) == -1)
			exit_with_error("pipe", data.child_pids);
		data.pid = fork();
		data.child_pids[data.index++] = data.pid;
		data.current->fd_in = data.current->fd[0];
		data.current->fd_out = data.current->fd[1];
		handle_all_process(&data, env);
		data.current = data.current->next;
	}
	// signal(SIGINT, SIG_IGN);
	printf("2 - je vais wait le pid\n");
	waitpid(data.pid, NULL, 0); // omg
	printf("3 - j'ai wait le pid\n");
	// signal(SIGINT, ft_builtin_ctrl_c);

	// wait_for_children(data.command, data.child_pids); // 
	// printf("3 - j'ai wait le pid\n");
	// clean_heredoc_files(current); // test
	cleanup(data.child_pids, data.infile);
}


/*
RAYAN
	int status;
	int i = 0;
	printf("nombre de pipes final : %d\n", command->nb_pipes);
	while (i < command->nb_pipes)
	{
		// printf("test\n");
		waitpid(0, &status, -1);
		i++;
	}
	WEXITSTATUS(status);
*/
