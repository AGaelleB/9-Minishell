heredoc_open_fd_pipe:
Pas de modifications majeures ici. 
Assurez-vous simplement de ne pas fermer heredoc_fd[0] 
immédiatement, car il sera utilisé comme entrée pour une commande ultérieure.



execve_fd:
Il semble que vous parcourez déjà toutes les commandes pour voir si 
l'une d'entre elles utilise un heredoc. C'est bien. 
Après avoir traité les heredocs, vous devez vous assurer de configurer 
le descripteur de fichier infile de data sur heredoc_fd[0] pour la commande qui 
utilise le heredoc comme entrée :

...
if (token->type == TYPE_HEREDOC)
{
	pid_t heredoc_pid = heredoc_open_fd_pipe(current, &token, data.heredoc_fd);
	waitpid(heredoc_pid, NULL, 0);
	data.infile = data.heredoc_fd[0];  // Ici, utilisez heredoc_fd comme entrée pour la commande
	flag = 1;
	break;
}
...
