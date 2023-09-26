minishell$> e'c'ho ' '        test
substr_list->str : e
command : e

substr_list->str : c
command : ec

substr_list->str : ho
command : echo

substr_list->str :  
command : echo 

substr_list->str :         test
command : echo         test

OK ici ca fontionne 

minishell$> 'echo' test
substr_list->str : echo
command : echo

substr_list->str : 
command : echo

substr_list->str : test
command : echotest

Faux, ici je devrai avoir : command : echo test

minishell$> 'echo' te's't
substr_list->str : echo
command : echo

substr_list->str : 
command : echo

substr_list->str : te
command : echote

substr_list->str : s
command : echotes

substr_list->str : t
command : echotest

Faux, ici je devrai avoir : command : echo test
