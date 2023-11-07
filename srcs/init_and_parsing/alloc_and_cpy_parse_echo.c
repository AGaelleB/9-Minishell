/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_and_cpy_parse_echo.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abonnefo <abonnefo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 10:51:04 by bfresque          #+#    #+#             */
/*   Updated: 2023/11/06 17:59:54 by abonnefo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

extern int	g_exit_status;

char *get_env_value_dollar(t_env *env, const char *key)
{
    int i = 0;
    char *key_eq = NULL;
    size_t key_len = strlen(key);

    // Construit une chaîne qui représente la clé suivie d'un signe égal pour la comparaison
    key_eq = malloc(key_len + 2); // +1 pour '=', +1 pour '\0'
    if (!key_eq) {
        // Gestion de l'erreur, retourne ou exit selon la convention de votre code
        return NULL;
    }
    sprintf(key_eq, "%s=", key);

    while (env->cpy_env[i])
    {
        // Si la clé est trouvée au début de la chaîne de l'environnement
        if (strncmp(env->cpy_env[i], key_eq, key_len + 1) == 0)
        {
            free(key_eq);
            // Retourne la valeur en sautant la clé et le signe égal
            return env->cpy_env[i] + key_len + 1;
        }
        i++;
    }

    free(key_eq); // Libère la mémoire de la chaîne temporaire
    return NULL; // Retourne NULL si la clé n'est pas trouvée
}



// Cette fonction devrait remplacer l'appel à append_env_value_to_arg
static void	append_env_value(char *var_name, char *arg, int *arg_idx, t_env *env)
{
	char	*value;

	// Obtenir la valeur de la variable d'environnement var_name
	value = get_env_value_dollar(env, var_name);
	if (value)
	{
		// Copier la valeur dans arg
		while (*value)
		{
			arg[(*arg_idx)++] = *value++;
		}
	}
}



void	handle_quotes_echo(char *str, int *i,
	bool *double_quote, bool *in_quote)
{
	if (!*in_quote && str[*i] == '\"' && str[*i + 1] == '\"')
		(*i) += 2;
	else if (!*double_quote && str[*i] == '\'' && str[*i + 1] == '\'')
		(*i) += 2;
	if (!*double_quote && str[*i] == '\'')
		*in_quote = !*in_quote;
	else if (!*in_quote && str[*i] == '\"')
		*double_quote = !*double_quote;
	if (!*in_quote && str[*i] == '\"')
		(*i)++;
	else if (!*double_quote && str[*i] == '\'')
		(*i)++;
}

static char	*get_exit_status_str(void)
{
	char	*str;

	str = ft_itoa(g_exit_status);
	if (!str)
		exit(EXIT_FAILURE);
	return (str);
}

static void handle_dollar(t_env *env, char *input, int *i, char *arg, int *arg_idx)
{
	char	*str;
	int		j;
	// int		start;

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
		*i += 1;
		j = *i;
		while (input[*i] && input[*i] != ' ' && input[*i] != '\'' && input[*i] != '\"')
			(*i)++;
		str = ft_substr(input, j, *i - j);
		if (!str)
			exit(EXIT_FAILURE);
		append_env_value(str, arg, arg_idx, env); // Replace append_env_value_to_arg
		free(str);
	}
}

// void	handle_arg_value(t_env *env, char *input, int *i, char *arg, int *arg_idx)
// {
// 	(void)env;
// 	if (input[*i] == '$')
// 		handle_dollar(input, i, arg, arg_idx);
// 	else
// 		arg[(*arg_idx)++] = input[(*i)++];
// }

void	handle_arg_value(t_env *env, char *input, int *i, char *arg, int *arg_idx)
{
	if (input[*i] == '$')
		handle_dollar(env, input, i, arg, arg_idx); // Pass env here
	else
		arg[(*arg_idx)++] = input[(*i)++];
}


// void	handle_arg_value(t_env *env, char *input, int *i, char *arg, int *arg_idx) // broken sur les exit status
// {
// 	char	*str;
// 	int		j;
// 	int		start;

// 	j = 0;
// 	start = *i + 1;
// 	while (input[*i] && input[*i] != ' ' && input[*i] != '\''
// 		&& input[*i] != '\"')
// 	{
// 		(*i)++;
// 		if (input[*i] != '\"' && input[*i] != '\'')
// 			j++;
// 	}
// 	str = ft_substr(input, start, j);
// 	append_env_value_to_arg(get_env_value(env, str), arg, arg_idx);
// }

char	*ft_allocate_and_copy(t_env *env, char *input, int *i, int *arg_idx)
{
	char	*arg;
	bool	double_quote;
	bool	single_quote;

	double_quote = false;
	single_quote = false;
	arg = malloc(sizeof(char *) * SIZE);
	if (!arg)
		exit(EXIT_FAILURE);
	*arg_idx = 0;
	while (input[*i] && (double_quote || single_quote || input[*i] != ' '))
	{
		handle_quotes_echo(input, i, &double_quote, &single_quote);
		if (is_redirection(input[*i]) && !double_quote && !single_quote)
			ft_skip_redirection_and_file(input, i);
		else if (input[*i] == '$' && !single_quote)
			handle_arg_value(env, input, i, arg, arg_idx);
		else
			arg[(*arg_idx)++] = input[(*i)++];
	}
	arg[*arg_idx] = '\0';
	skip_spaces_echo(input, i);
	return (arg);
}
