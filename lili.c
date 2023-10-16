int ft_builtin_cd(char **args, t_env *env)
{
    char *home;
    char *path_to_change;
    char *new_directory;

    if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
    {
        home = get_home_directory();
        if (!home)
        {
            write(2, "minishell: cd: HOME not set\n", 29);
            return (1);
        }
        path_to_change = home;
    }
    else
        path_to_change = args[1];
    update_env_oldpwd(env);
    if (chdir(path_to_change) == -1)
    {
        fprintf(stderr, "minishell: cd: %s: ", path_to_change); //change that WARNING !!!!!!
        perror("");
        return (1);
    }
    new_directory = getcwd(NULL, 0);
    if (new_directory)
    {
        update_env_pwd(env, new_directory);
        free(new_directory);
    }
    return (0);
}
