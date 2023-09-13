I still have 1 fd open, where should I close ot ? 

valgrind --leak-check=full --trace-children=yes --track-fds=yes ./minishell
==738439== Memcheck, a memory error detector
==738439== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==738439== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==738439== Command: ./minishell
==738439== 
minishell$> cat test.txt | rev
==738447== Memcheck, a memory error detector
==738447== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==738447== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==738447== Command: /usr/bin/cat test.txt
==738447== 
==738448== Memcheck, a memory error detector
==738448== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==738448== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==738448== Command: /usr/bin/rev
==738448== 
stom uocuoc
==738447== 
==738447== FILE DESCRIPTORS: 1 open (1 std) at exit.
==738447== 
==738447== HEAP SUMMARY:
==738447==     in use at exit: 0 bytes in 0 blocks
==738447==   total heap usage: 31 allocs, 31 frees, 139,232 bytes allocated
==738447== 
==738447== All heap blocks were freed -- no leaks are possible
==738447== 
==738447== For lists of detected and suppressed errors, rerun with: -s
==738447== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 tse'c
eriomém
stom
stom sel

stom ed puocuaeb
==738448== 
==738448== FILE DESCRIPTORS: 4 open (3 std) at exit.
==738448== Open file descriptor 3:
==738448==    <inherited from parent>
==738448== 
==738448== 
==738448== HEAP SUMMARY:
==738448==     in use at exit: 0 bytes in 0 blocks
==738448==   total heap usage: 38 allocs, 38 frees, 62,883 bytes allocated
==738448== 
==738448== All heap blocks were freed -- no leaks are possible
==738448== 
==738448== For lists of detected and suppressed errors, rerun with: -s
==738448== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
minishell$> cat test.txt | rev | wc
==738460== Memcheck, a memory error detector
==738460== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==738460== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==738460== Command: /usr/bin/cat test.txt
==738460== 
==738461== Memcheck, a memory error detector
==738461== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==738461== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==738461== Command: /usr/bin/rev
==738461== 
==738462== Memcheck, a memory error detector
==738462== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==738462== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==738462== Command: /usr/bin/wc
==738462== 
==738460== 
==738460== FILE DESCRIPTORS: 1 open (1 std) at exit.
==738460== 
==738460== HEAP SUMMARY:
==738460==     in use at exit: 0 bytes in 0 blocks
==738460==   total heap usage: 31 allocs, 31 frees, 139,232 bytes allocated
==738460== 
==738460== All heap blocks were freed -- no leaks are possible
==738460== 
==738460== For lists of detected and suppressed errors, rerun with: -s
==738460== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
==738461== 
==738461== FILE DESCRIPTORS: 4 open (3 std) at exit.
==738461== Open file descriptor 3:
==738461==    <inherited from parent>
==738461== 
==738461== 
==738461== HEAP SUMMARY:
==738461==     in use at exit: 0 bytes in 0 blocks
==738461==   total heap usage: 38 allocs, 38 frees, 78,243 bytes allocated
==738461== 
==738461== All heap blocks were freed -- no leaks are possible
==738461== 
==738461== For lists of detected and suppressed errors, rerun with: -s
==738461== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
      7      10      60
==738462== 
==738462== FILE DESCRIPTORS: 1 open (0 std) at exit.
==738462== Open file descriptor 4:
==738462==    <inherited from parent>
==738462== 
==738462== 
==738462== HEAP SUMMARY:
==738462==     in use at exit: 0 bytes in 0 blocks
==738462==   total heap usage: 152 allocs, 152 frees, 21,882 bytes allocated
==738462== 
==738462== All heap blocks were freed -- no leaks are possible
==738462== 
==738462== For lists of detected and suppressed errors, rerun with: -s
==738462== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
minishell$> 


void child_process(t_command *current, char **envp)
{
	if (current->fd[0] != -1)
	{
		dup2(current->fd[0], 0); // Redirect stdin
		close(current->fd[0]);     // Close read_fd as it's not needed anymore
	}

	if (current->fd[1] != -1)
	{
		dup2(current->fd[1], 1); // Redirect stdout
		close(current->fd[1]);     // Close write_fd as it's not needed anymore
	}

	ft_set_args_and_paths(current, envp);
	if (current->command_path == NULL)
	{
		write(2, "minishell: command not found: ", 31);
		write(2, current->command_arg[0], ft_strlen(current->command_arg[0]));
		write(2, "\n", 1);
		exit(127); // Standard exit code for "command not found"
	}
	else if (execve(current->command_path, current->command_arg, envp) == -1)
	{
		perror("Error");
		exit(-1);
	}
}

void open_fd(t_command *current, char **envp)
{
	t_command *command;
	pid_t pid;
	pid_t *child_pids;
	int infile; // Used to maintain the read-end of the pipe
	int num_children;
	int index;

	command = current;
	infile = 0;
	num_children = 0;
	index = 0;
	while (current)
	{
		num_children++;
		current = current->next;
	}
	current = command;
	child_pids = malloc(num_children * sizeof(pid_t));
	if (!child_pids)
		exit(1); // Memory allocation failure
	while (current)
	{
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
			child_process(current, envp);
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
	for (int i = 0; i < num_children; i++)
	{
		waitpid(child_pids[i], NULL, 0);
	}
	free(child_pids);
	// Close the last remaining read-end of the pipe in the parent process
	if (infile != 0)
		close(infile);
}
