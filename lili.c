still not working, redirect_file_out and redirect_append_file_out are working well put redirect_file_in no.
I don't understand why fot redirect_file_out and redirect_append_file_out it's woking, the scurrent->fd_out change well, but for redirect_file_in, the current->fd_in is unchanged and seems to be block a current->fd_in = 3 

./minishell 
minishell$> cat < test.txt 
TYPE_REDIR_IN
filemane : test.txt
current->fd_in AVANT 3
current->fd_in APRES 3
fini
Error: Bad address
minishell$> echo bonjour >> test.txt 
current->fd_out AVANT 4
current->fd_out AVANT 3
minishell$> echo bjr > test.txt 
current->fd_out AVANT 4
current->fd_out APRES 3
minishell$> 