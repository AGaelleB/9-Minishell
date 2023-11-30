
help me to fix my problem with add_spaces_around_redir

char	*add_spaces_around_redir(char *input)
{
	char	*new_input;
	int		new_len;

	new_len = calculate_new_len(input);
	new_input = malloc(sizeof(char) * (new_len + 1));
	if (!new_input)
	{
		perror("malloc");
		return (NULL);
	}
	return (copy_with_spaces(input, new_input));
}

t_command	*create_new_cmd(char *command_str, t_env *env)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->command = add_spaces_around_redir(command_str);
	if (!new_cmd->command)
		return (NULL);
	new_cmd->token_head = tokenize_input(new_cmd->command, env);
	if (new_cmd->token_head != NULL)
	{
		new_cmd->token_head->command = NULL;
		new_cmd->token_head->command = add_spaces_around_redir(command_str);
		new_cmd->token_head->command_two = add_spaces_around_redir(command_str);
	}
	new_cmd->next = NULL;
	return (new_cmd);
}

t_command	*get_command(char *input, t_env *env)
{
	t_command	*head;
	t_command	*new_cmd;
	char		**command;
	int			i;

	head = NULL;
	command = split_command_on_pipe(input);
	i = 0;
	while (command[i])
	{
		if (ft_strcmp_minishell(command[i], "") == 0)
			return (NULL);
		new_cmd = create_new_cmd(command[i], env);
		new_cmd->heredocs = NULL;
		new_cmd->command_arg = NULL;
		new_cmd->command_arg_main = NULL;
		new_cmd->export_arg = NULL;
		append_new_cmd(&head, new_cmd);
		i++;
	}
	ft_free_tab(command);
	return (head);
}


void	main_loop(t_env *env_bis)
{
	t_process_data	data_main;
	t_command	*new_cmd;
	char		*input;
	int			flag_ok;

	flag_ok = 0;
	while (1)
	{
		input = readline("minishell$> ");
		add_history(input);
		ctrl_d_main(input, new_cmd, env_bis, flag_ok);
		// ft_builtin_write_exit(input);
		if (error_input(env_bis, new_cmd, input, flag_ok) == 2
			|| verif_nb_quote(input) != 0
			|| pipe_syntax_errors(input) == 2)
			continue ;
		new_cmd = get_command(input, env_bis);
		count_and_set_pipes(input, new_cmd);
		if (new_cmd != NULL)
		{
			new_cmd->command_arg_main = parse_input_quote(new_cmd->command);
			new_cmd->export_arg = parse_arg_export(new_cmd->command);
			execve_builtins_unset_export(new_cmd, env_bis);
			execve_builtin_cd(new_cmd, env_bis);
			data_main = execve_fd(new_cmd, env_bis);
			if (new_cmd->command_arg_main)
				ft_free_tab(new_cmd->command_arg_main);
			(void)data_main;
			if (new_cmd->token_head)
				ft_free_token(new_cmd);
			if (new_cmd)
				ft_free_current(new_cmd);
		}
		flag_ok = 1;
		free(input);
	}
	ft_free_env(env_bis);
	ft_free_all(new_cmd, new_cmd->token_head);
	clear_history();
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env_bis;

	if (check_args_and_env(ac, envp))
		return (1);
	env_bis = initialize_env(envp);
	if (!env_bis)
		return (1);
	main_loop(env_bis);
	(void)av;
	return (0);
}

minishell$> echo "c"
==5677== Conditional jump or move depends on uninitialised value(s)
==5677==    at 0x10A40F: allocate_and_copy (handle_quotes.c:53)
==5677==    by 0x10A4BE: copy_argument (handle_quotes.c:78)
==5677==    by 0x10A5CF: parse_input_quote (handle_quotes.c:105)
==5677==    by 0x10961F: main_loop (main_minishell.c:63)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677==  Uninitialised value was created by a heap allocation
==5677==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5677==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5677==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5677==    by 0x10DBA4: get_command (create_cmd.c:69)
==5677==    by 0x1095ED: main_loop (main_minishell.c:59)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677== 
==5677== Conditional jump or move depends on uninitialised value(s)
==5677==    at 0x10A47B: skip_spaces (handle_quotes.c:69)
==5677==    by 0x10A525: copy_argument (handle_quotes.c:85)
==5677==    by 0x10A5CF: parse_input_quote (handle_quotes.c:105)
==5677==    by 0x10961F: main_loop (main_minishell.c:63)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677==  Uninitialised value was created by a heap allocation
==5677==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5677==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5677==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5677==    by 0x10DBA4: get_command (create_cmd.c:69)
==5677==    by 0x1095ED: main_loop (main_minishell.c:59)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677== 
==5677== Conditional jump or move depends on uninitialised value(s)
==5677==    at 0x10A601: parse_input_quote (handle_quotes.c:108)
==5677==    by 0x10961F: main_loop (main_minishell.c:63)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677==  Uninitialised value was created by a heap allocation
==5677==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5677==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5677==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5677==    by 0x10DBA4: get_command (create_cmd.c:69)
==5677==    by 0x1095ED: main_loop (main_minishell.c:59)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677== 
==5677== Conditional jump or move depends on uninitialised value(s)
==5677==    at 0x10A615: parse_input_quote (handle_quotes.c:109)
==5677==    by 0x10961F: main_loop (main_minishell.c:63)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677==  Uninitialised value was created by a heap allocation
==5677==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5677==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5677==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5677==    by 0x10DBA4: get_command (create_cmd.c:69)
==5677==    by 0x1095ED: main_loop (main_minishell.c:59)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677== 
==5677== Conditional jump or move depends on uninitialised value(s)
==5677==    at 0x10A629: parse_input_quote (handle_quotes.c:103)
==5677==    by 0x10961F: main_loop (main_minishell.c:63)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677==  Uninitialised value was created by a heap allocation
==5677==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5677==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5677==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5677==    by 0x10DBA4: get_command (create_cmd.c:69)
==5677==    by 0x1095ED: main_loop (main_minishell.c:59)
==5677==    by 0x109740: main (main_minishell.c:101)
==5677== 
==5695== Conditional jump or move depends on uninitialised value(s)
==5695==    at 0x10D216: process_input (alloc_and_cpy_parse_echo.c:38)
==5695==    by 0x10D307: ft_allocate_and_copy (alloc_and_cpy_parse_echo.c:71)
==5695==    by 0x10F986: copy_argument_echo (parsing_echo.c:33)
==5695==    by 0x10FA9C: parse_input_quote_echo (parsing_echo.c:58)
==5695==    by 0x10C9D7: check_echo (check_builtins_1.c:34)
==5695==    by 0x10B096: builtins_verif (all_builtins_verifs.c:132)
==5695==    by 0x10A0E7: handle_child_process (handle_process.c:55)
==5695==    by 0x10A1C9: handle_all_process (handle_process.c:77)
==5695==    by 0x10AC1C: handle_execve_processes (read_fd.c:49)
==5695==    by 0x10ACD8: execve_fd (read_fd.c:67)
==5695==    by 0x10967C: main_loop (main_minishell.c:68)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695==  Uninitialised value was created by a heap allocation
==5695==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5695==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5695==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5695==    by 0x10DBA4: get_command (create_cmd.c:69)
==5695==    by 0x1095ED: main_loop (main_minishell.c:59)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695== 
==5695== Conditional jump or move depends on uninitialised value(s)
==5695==    at 0x10F91B: skip_spaces_echo (parsing_echo.c:17)
==5695==    by 0x10D31E: ft_allocate_and_copy (alloc_and_cpy_parse_echo.c:72)
==5695==    by 0x10F986: copy_argument_echo (parsing_echo.c:33)
==5695==    by 0x10FA9C: parse_input_quote_echo (parsing_echo.c:58)
==5695==    by 0x10C9D7: check_echo (check_builtins_1.c:34)
==5695==    by 0x10B096: builtins_verif (all_builtins_verifs.c:132)
==5695==    by 0x10A0E7: handle_child_process (handle_process.c:55)
==5695==    by 0x10A1C9: handle_all_process (handle_process.c:77)
==5695==    by 0x10AC1C: handle_execve_processes (read_fd.c:49)
==5695==    by 0x10ACD8: execve_fd (read_fd.c:67)
==5695==    by 0x10967C: main_loop (main_minishell.c:68)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695==  Uninitialised value was created by a heap allocation
==5695==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5695==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5695==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5695==    by 0x10DBA4: get_command (create_cmd.c:69)
==5695==    by 0x1095ED: main_loop (main_minishell.c:59)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695== 
==5695== Conditional jump or move depends on uninitialised value(s)
==5695==    at 0x10F91B: skip_spaces_echo (parsing_echo.c:17)
==5695==    by 0x10F9ED: copy_argument_echo (parsing_echo.c:40)
==5695==    by 0x10FA9C: parse_input_quote_echo (parsing_echo.c:58)
==5695==    by 0x10C9D7: check_echo (check_builtins_1.c:34)
==5695==    by 0x10B096: builtins_verif (all_builtins_verifs.c:132)
==5695==    by 0x10A0E7: handle_child_process (handle_process.c:55)
==5695==    by 0x10A1C9: handle_all_process (handle_process.c:77)
==5695==    by 0x10AC1C: handle_execve_processes (read_fd.c:49)
==5695==    by 0x10ACD8: execve_fd (read_fd.c:67)
==5695==    by 0x10967C: main_loop (main_minishell.c:68)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695==  Uninitialised value was created by a heap allocation
==5695==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5695==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5695==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5695==    by 0x10DBA4: get_command (create_cmd.c:69)
==5695==    by 0x1095ED: main_loop (main_minishell.c:59)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695== 
==5695== Conditional jump or move depends on uninitialised value(s)
==5695==    at 0x10FAC3: parse_input_quote_echo (parsing_echo.c:56)
==5695==    by 0x10C9D7: check_echo (check_builtins_1.c:34)
==5695==    by 0x10B096: builtins_verif (all_builtins_verifs.c:132)
==5695==    by 0x10A0E7: handle_child_process (handle_process.c:55)
==5695==    by 0x10A1C9: handle_all_process (handle_process.c:77)
==5695==    by 0x10AC1C: handle_execve_processes (read_fd.c:49)
==5695==    by 0x10ACD8: execve_fd (read_fd.c:67)
==5695==    by 0x10967C: main_loop (main_minishell.c:68)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695==  Uninitialised value was created by a heap allocation
==5695==    at 0x4848899: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==5695==    by 0x10F37F: add_spaces_around_redir (parse_spaces_redir.c:66)
==5695==    by 0x10D9F9: create_new_cmd (create_cmd.c:22)
==5695==    by 0x10DBA4: get_command (create_cmd.c:69)
==5695==    by 0x1095ED: main_loop (main_minishell.c:59)
==5695==    by 0x109740: main (main_minishell.c:101)
==5695== 
c