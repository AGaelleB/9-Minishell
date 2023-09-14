I've tried to decalre my global variable in my .h because I use 2 files .c, but it doesn't __OPENCL_MEMORY_SCOPE_WORK_GROUP
How can I fix my problem ? 

my header.h : 


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


volatile int	ctrl_c_pressed = 0;

 // rest of the doc

/////////////////////////

my file number1.c  : 

#include "../../includes/header.h"

t_command	*new_commands = NULL;

void cleanup_resources(void)
{
	if (new_commands)
	{
		close(new_commands->fd[1]);
		close(new_commands->fd[0]);
		ft_free_all_cmd(new_commands);
		free(new_commands);
		new_commands = NULL;
	}
}

int main(int ac, char **av, char **envp)
{
	char		*input;
	int			builtin_status;

	signal(SIGINT, ft_signal_ctrl_C);
	while (1)
	{

        ctrl_c_pressed = 0; // Reset the flag at the start of each loop
        input = readline("minishell$> ");

        if (ctrl_c_pressed) // If Ctrl+C was pressed, just free the input and loop back
        {
            free(input);
            continue;
        }
				ft_builtin_ctrl_D(input);
		builtin_status = ft_all_builtins_exit(input);
		if (builtin_status == 1)
		{
			free(input);
			cleanup_resources(); // NEW
			exit(0);
		}
		else if (builtin_status == 2)
			continue;
		new_commands = get_command(input);
		count_and_set_pipes(input, new_commands);
		if(new_commands != NULL)
		{
			new_commands->token = tokenize_input(input);
			open_fd(new_commands, envp);
		}
		add_history(input);
		free(input);
	}
	(void)ac;
	(void)av;
	(void)envp;
	return (0);
}

/////////////////////////

my file number2.c  : 

#include "../../includes/header.h"

// volatile int	g_unblock_sigquit = 0;

// volatile int	ctrl_c_pressed = 0;

void ft_signal_ctrl_C(int signal)
{
    (void)signal;
    cleanup_resources(); // Cleanup any resources if needed
    write(1, "\nminishell$> ", 13);
	ctrl_c_pressed = 1; // Indicate that Ctrl+C was pressed
}

/////////////////////////

the error : 

 make re
[.o] Object files removed
[.a] Binary file removed
/usr/bin/ld: srcs/obj_minishell/srcs/builtins/all_builtins.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/builtins/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/builtins/builtins_exit.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/builtins/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/execution/execve.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/execution/read_fd.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/init_and_parsing/init_token.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/init_and_parsing/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/init_and_parsing/path.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/init_and_parsing/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/signals/signal_exit.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/signals/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/utils/libft_modify.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/utils/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/utils/free.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/utils/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/utils/error.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/utils/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
/usr/bin/ld: srcs/obj_minishell/srcs/utils/utils.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/utils/../../includes/minishell.h:38: multiple definition of `ctrl_c_pressed'; srcs/obj_minishell/srcs/execution/main_minishell.o:/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/srcs/execution/../../includes/minishell.h:38: first defined here
collect2: error: ld returned 1 exit status
make: *** [Makefile:108: minishell] Error 1