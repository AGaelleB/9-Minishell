volatile sig_atomic_t heredoc_interrupted = 0;

void ctrl_c_heredoc(int signal) {
    if (signal == SIGINT) {
        heredoc_interrupted = 1;
        printf("\n");
    }
}

int here_doc_manage(...) {
    // ... Votre code existant ...
    if (heredoc_interrupted) {
        // Si Ctrl+C a été pressé, fermez le fd et terminez
        close(fd[1]);
        exit(130);  // Utilisez un code de sortie spécifique
    }
    // ... Reste du code ...
}

int here_doc_rayan(...) {
    // ... Initialisation ...
    signal(SIGINT, ctrl_c_heredoc); // Configurez le gestionnaire de signal
    heredoc_interrupted = 0;

    while (i < data->count_hd) {
        // ... Votre code existant pour créer des processus enfants ...
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 130) {
            // Si un heredoc a été interrompu, arrêtez d'attendre les autres
            break;
        }
        // ... Reste du code ...
    }

    // Restaurer le gestionnaire de signal par défaut ou comme avant
    signal(SIGINT, SIG_DFL);
    return heredoc_interrupted ? -1 : 0; // Retournez -1 si interrompu
}

// Dans handle_execve_processes ou ailleurs, gérez le cas où heredoc est interrompu