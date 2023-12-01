#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define SIZE 256 // Replace with the appropriate size

// Define your structures here (t_env, t_process_data, t_command, etc.)

// Function prototypes
int check_args_and_env(int ac, char **envp);
t_env *initialize_env(char **envp);
void main_loop(t_env *env_bis);

// Other function prototypes for functions used within main_loop
static void read_and_process_input(t_env *env_bis);
static void process_command(t_command *new_cmd, t_env *env_bis);
static void cleanup_command(t_command *new_cmd);

int main(int ac, char **av, char **envp)
{
    t_env *env_bis;

    if (check_args_and_env(ac, envp))
        return (1);
    env_bis = initialize_env(envp);
    if (!env_bis)
        return (1);
    main_loop(env_bis);
    (void)av;
    return (0);
}

int check_args_and_env(int ac, char **envp)
{
    if (ac != 1)
        return (printf("run ./minishell without arg\n"));
    if (!envp[0])
        return (printf("env is missing\n"));
    return (0);
}

t_env *initialize_env(char **envp)
{
    t_env *env_bis;

    g_exit_status = 0;
    signal(SIGINT, ctrl_c_main);
    signal(SIGQUIT, SIG_IGN);
    env_bis = (t_env *)malloc(sizeof(t_env));
    if (!env_bis)
        return (NULL);
    return_env(env_bis);
    copy_env(env_bis, envp);
    return (env_bis);
}

void main_loop(t_env *env_bis)
{
    read_and_process_input(env_bis);
    ft_free_env(env_bis);
    clear_history();
}

static void read_and_process_input(t_env *env_bis)
{
    t_process_data data_main;
    t_command *new_cmd;
    char *input;
    int flag_ok;

    flag_ok = 0;
    while (1)
    {
        input = readline("minishell$> ");
        add_history(input);
        ctrl_d_main(input, new_cmd, env_bis, flag_ok);
        if (error_input(env_bis, input) == 2 ||
            verif_nb_quote(input) != 0 ||
            pipe_syntax_errors(input) == 2)
            continue;
        new_cmd = get_command(input, env_bis);
        count_and_set_pipes(input, new_cmd);
        if (new_cmd != NULL)
        {
            process_command(new_cmd, env_bis);
            cleanup_command(new_cmd);
        }
        flag_ok = 1;
        free(input);
    }
}

static void process_command(t_command *new_cmd, t_env *env_bis)
{
    new_cmd->command_arg_main = parse_input_quote(new_cmd->command);
    new_cmd->export_arg = parse_arg_export(new_cmd->command);
    execve_builtins_unset_export(new_cmd, env_bis);
    execve_builtin_cd(new_cmd, env_bis);
    t_process_data data_main = execve_fd(new_cmd, env_bis);
    if (new_cmd->command_arg_main)
        ft_free_tab(new_cmd->command_arg_main);
    (void)data_main;
    if (new_cmd->token_head)
        ft_free_token(new_cmd);
}

static void cleanup_command(t_command *new_cmd)
{
    if (new_cmd)
        ft_free_current(new_cmd);
}
