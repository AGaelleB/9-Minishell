void ft_free_token(t_command *current)
{
    while (current)
	{
        t_token *head = current->token_head;
        while (head)
		{
            t_token *tmp = head;
            head = head->next;
            free(tmp->split_value);
            free(tmp);
        }
        current->token_head = NULL;  // Set token_head to NULL after freeing all tokens
        current = current->next;
    }
}

void ft_free_current(t_command *head)
{
    while (head)
	{
        ft_free_tab(head->command_arg);
        free(head->command_path);
        free(head->command);
        t_command *tmp = head;
        head = head->next;
        free(tmp);
    }
}
// void	ft_free_token(t_command *current, t_token *head)
// {
// 	t_token	*tmp;

// 	tmp = NULL;
// 	while (current)
// 	{
// 		head = current->token_head;
// 		if (head != NULL)
// 		{
// 			while (head)
// 			{
// 				tmp = head;
// 				// head->command = NULL;
// 				// if (tmp->command != NULL)
// 				// 	free(tmp->command);
// 				free(tmp->split_value);
// 				// free(tmp);
// 				head = head->next;
// 			}
// 			free(current->token_head->command);
// 			if(current->token_head != NULL)
// 				free(current->token_head);
// 			// if(head != NULL) //visiblement inutile
// 			// 	free(head);
// 		}
// 		current = current->next;
// 	}
// }

