int expand_variable(t_export *export, char **str, char *str_cpy, t_env *env) {
    while (1) {
        export->k = 0;
        char *var_name = extract_var_name(*str);
        export->j = find_env_var(env, var_name);

        if (export->j != -1) {
            begin_var(export, *str);
            copy_env_in_return(export, env);
            *str = re_init_var_str(export, *str);

            if (!(*str)) {
                free(var_name);
                return 0; // Indique que la fonction doit se terminer
            }
        } else {
            *str = check_none_var(*str);
            free(var_name);
            return 0; // Indique que la fonction doit se terminer
        }
        free(var_name);
    }
}

int export_expander(t_export *export, char *var_name, char *str, t_env *env) {
    char *str_cpy = ft_strdup(str);

    if (expand_variable(export, &str, str_cpy, env) == 0) {
        update_var_env(env, str_cpy);
        export->ret[export->l] = '\0';
        add_var_env(env, export->i, export->ret);
        free(str_cpy);
        free(str);
        // Libérer la structure export si nécessaire
        return (g_exit_status);
    }
    // Libérer la structure export si nécessaire
    return (g_exit_status);
}
