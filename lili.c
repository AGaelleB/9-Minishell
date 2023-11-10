static void append_env_value(t_arg_handler *arg_handler, char *env_value)
{
    printf("%s append_env_value env value = %s%s\n", RED, env_value, RESET);
    while (*env_value)
    {
        arg_handler->arg[(*arg_handler->arg_idx)++] = *env_value++;
    }
}

static void handle_dollar(t_arg_handler *arg_handler)
{
    char *str;
    char *env_value;
    int start = *arg_handler->i + 1;

    if (arg_handler->input[start] == '?')
    {
        str = get_exit_status_str();
        append_env_value(arg_handler, str);
        free(str);
        *arg_handler->i += 2;
    }
    else
    {
        (*arg_handler->i)++;
        extract_variable_name(arg_handler->input, arg_handler->i, &start, &str);
        env_value = get_env_value(arg_handler->env, str);
        printf("%s handle_dollar env value = %s%s\n", BLUE, env_value, RESET);
        if (env_value)
        {
            append_env_value(arg_handler, env_value);
            free(env_value); // Assuming get_env_value allocates memory for env_value
        }
        free(str); // Freeing the result of extract_variable_name
    }
}
