minishell$> echo bonjour > > out
c = >
bash: syntax error near unexpected token o

void	verif_file_name(t_token *token)
{
	int		i;
	char	c;

	i = 0;
	c = token->next->split_value[i];
	printf("c = %c\n", c);
	if(check_valid_identifier(c) == 0)
	{
		ft_putstr_fd("bash: syntax error near unexpected token ", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("\n", 2);
		exit(-1);
	}
}