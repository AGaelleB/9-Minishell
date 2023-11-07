char **parse_input_quote(char *input)
{
    t_parser parser;
    int arg_count;

    // Consider both single and double quotes in the count
    arg_count = count_args_single_quotes(input) + count_args_double_quotes_args(input);
    parser.args = malloc((arg_count + 1) * sizeof(char *));
    parser.i = 0;
    parser.idx = 0;
    parser.in_quote = false;

    if (!parser.args) {
        // Handle malloc failure
        return NULL;
    }

    bool in_single_quote = false;
    bool in_double_quote = false;
	
    while (input[parser.i] != '\0')
	{
        handle_quotes(input, &parser.i, &in_single_quote, '\'');
        handle_quotes(input, &parser.i, &in_double_quote, '\"');

        // Detect the end of an argument when a space is found outside quotes
        if (input[parser.i] == ' ' && !in_single_quote && !in_double_quote)
		{
            // Allocate and copy the argument here
            parser.args[parser.idx++] = allocate_and_copy(input, &parser.i, &arg_idx, in_single_quote, in_double_quote);
            // Skip spaces after the argument
            while (input[parser.i] == ' ') parser.i++;
        } else if (input[parser.i] != ' ' || in_single_quote || in_double_quote) {
            // Continue handling characters within the argument
            parser.i++;
        }
    }

    // Handle the last argument if there is one
    if (parser.i != 0 && (in_single_quote || in_double_quote || input[parser.i - 1] != ' ')) {
        parser.args[parser.idx++] = allocate_and_copy(input, &parser.i, &arg_idx, in_single_quote, in_double_quote);
    }

    // Null-terminate the arguments array
    parser.args[parser.idx] = NULL;

    return parser.args;
}