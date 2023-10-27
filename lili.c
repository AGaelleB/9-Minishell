if (global_ctrl_c_pressed == 130)
{
    clean_heredoc_files(current);
    free(line);
    return (130); // Return error code for CTRL+C
}


// Partie de votre boucle principale
...
new_commands = get_command(input, env_bis);
...
if (new_commands != NULL)
{
    ...
    int ret = execve_fd(new_commands, env_bis);
    if (ret == 130) // Si CTRL+C a été pressé dans le heredoc
    {
        // Vous pouvez afficher un message ou simplement continuer
    }
    ...
}
...


void execve_fd(t_command *current, t_env *env)
{
    ...
    int ret = write_in_fd(...);
    if (ret != 0)
    {
        return ret;
    }
    ...
}
