void handle_quotes_echo(char *str, int *i, bool *double_quote, bool *single_quote)
{
    // Si nous trouvons une paire de guillemets doubles consécutifs, nous les sautons.
    if (str[*i] == '\"' && str[*i + 1] == '\"' && !*single_quote) {
        (*i) += 2;
        return;
    }
    // Si nous trouvons une paire de guillemets simples consécutifs, nous les sautons.
    else if (str[*i] == '\'' && str[*i + 1] == '\'' && !*double_quote) {
        (*i) += 2;
        return;
    }

    // Si nous trouvons un guillemet simple, nous basculons l'état de single_quote.
    if (str[*i] == '\'' && !*double_quote) {
        *single_quote = !*single_quote;
        (*i)++;
        return;
    }
    // Si nous trouvons un guillemet double, nous basculons l'état de double_quote.
    else if (str[*i] == '\"' && !*single_quote) {
        *double_quote = !*double_quote;
        (*i)++;
        return;
    }
}

char *ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
    t_arg_handler	handler;
    bool			double_quote = false;
    bool			single_quote = false;

    handler = init_handler(env, input, i, arg_idx);
    handler.arg = malloc(sizeof(char) * SIZE);  // Attention à la taille allouée.
    if (!handler.arg)
        exit(EXIT_FAILURE);

    *handler.arg_idx = 0;
    while (handler.input[*handler.i])
    {
        if (handler.input[*handler.i] == ' ' && !double_quote && !single_quote) {
            break;  // Arrêt si espace et pas dans les guillemets.
        }

        handle_quotes_echo(handler.input, handler.i, &double_quote, &single_quote);
        if (is_redirection(handler.input[*handler.i]) && !double_quote && !single_quote) {
            ft_skip_redirection_and_file(handler.input, handler.i);
        } else if (handler.input[*handler.i] == '$' && !single_quote) {
            handle_arg_value(&handler);
        } else {
            handler.arg[(*handler.arg_idx)++] = handler.input[*handler.i];
            (*handler.i)++;
        }
    }

    handler.arg[*handler.arg_idx] = '\0';
    return (handler.arg);
}
