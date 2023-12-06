static int handle_input(char *input, int i, char *delimiter, int fd[2]) {
    // ... (reste du code)

    // Supprimer le retour à la ligne de input.
    int input_len = ft_strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    if (ft_strcmp_hd(input, delimiter) == 0) {
        // ... (reste du code)
    }

    // ... (reste du code)
}
