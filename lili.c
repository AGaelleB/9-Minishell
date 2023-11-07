static void handle_dollar(t_env *env, char *input, int *i, char *arg, int *arg_idx)
{
    // ... other code ...

    if (input[*i + 1] == '?')
    {
        char *str = get_exit_status_str(); // This should return a string version of g_exit_status
        for (int j = 0; str[j]; j++)
        {
            arg[(*arg_idx)++] = str[j]; // Copy the exit status to the arg
        }
        free(str); // Free the string if necessary
        *i += 2; // Move past the '$' and '?'
    }
    else
    {
        // ... code for handling other variables ...
    }
}

// This assumes g_exit_status is a global int variable set elsewhere in the code
static char *get_exit_status_str(void)
{
    // The buffer must be large enough to hold any possible int value and the terminating null
    static char str[12]; // 11 characters for INT_MIN and 1 for the null terminator
    sprintf(str, "%d", g_exit_status);
    return str; // No need to malloc or free as we return a static buffer
}
