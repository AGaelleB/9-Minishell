TEST
tableaux excel : https://github.com/Kwevan/42-Tests


*** TEST DES SINGLES QUOTES : ***

bash-5.1$ echo' test'
bash: echo test: command not found

bash-5.1$ echo' 'test
bash: echo test: command not found

bash-5.1$ echo test ls
test ls

bash-5.1$ echo ' 'ls
 ls

bash-5.1$ echo 'debut              fin'       test
debut              fin test

bash-5.1$  echo 1 2 3 4 5
1 2 3 4 5

bash-5.1$ ec'h'o     t'rst'
trst

bash-5.1$ ec'h'o'   ' t'rst'
bash: echo   : command not found

bash-5.1$ 'ech'o' c'oucou
bash: echo coucou: command not found

bash-5.1$ 'ech'o     '  '  coucou
   coucou



   *** ECHO -n : ***

bash-5.1$ echo -n -n -nnnn -nnnnabc
-nnnnabcbash-5.1$ 

bash-5.1$ echo -n blabla
blablabash-5.1$ 

minishell$> echo -a -nnn -er -nnnnnn -nae -nnn  bonjour
-a -nnn -er -nnnnnn -nae -nnn bonjour

bash-5.1$ echo -nnnn -n -nnnq ghg 'gfdgd' ' a       f'
-nnnq ghg gfdgd  a       fbash-5.1$

bash-5.1$ echo -n' 'test
-n test

bash-5.1$ echo -nnna
-nnna

bash-5.1$ echo 'test > oups'
test > oups

bash-5.1$ echo 'test>oups'
test>oups

bash-5.1$ echo 'tes't>oups
bash-5.1$ cat oups 
test

bash-5.1$ echo -nnnn -n -nnnq ghg 'gfdgd'>' a       f'  //////////////////// WARNING WARNING
   -> ici on doit renvoyer dans le fichier 'a       f'  "-nnnq ghg gfdgd"

bash-5.1$ echo 'je casse'>'tonshell'  //////////////////// WARNING WARNING
   -> ici on doit renvoyer dans le fichier 'tonshell' (sans single quote)  "je casse"

bash-5.1$ echo 'je casse'>'ton shell'  //////////////////// WARNING WARNING
   -> ici on doit renvoyer dans le fichier 'tonshell' (sans single quote)  "je casse"

minishell$>  cat celine.txt | echo 'coucou>t'est>a>b>>olalala
   -> ici on doit renvoyer dans le fichier 'olalala' "coucou>test" en append

minishell$>  cat celine.txt | echo 'coucout'est>a>b>>olalala
   -> ici on doit renvoyer dans le fichier 'olalala' "coucoutest" en append

bash-5.1$ echo test>oups
bash-5.1$ cat oups 
test

bash-5.1$ echo test'>'oups
test>oups

bash-5.1$ cat celine.txt | echo 'coucou>t'est>a>b>>c
   -> ici on doit renvoyer dans le fichier 'c' "coucou>test"

bash-5.1$ echo t'est >'> olalala
   -> ici on doit renvoyer dans le fichier 'olalala' "test >"



   *** TEST DES REDIRECTIONS : ***

bash-5.1$            >a>   b>c   cat '  '   celine.txt
cat: '  ': No such file or directory
   -> doit aussi renvoyer le cat dans le fichier 

bash-5.1$ > a > b cat celine.txt
   -> doit renvoyer le cat dans le fichier 

bash-5.1$ >a>b cat celine.txt
   -> doit renvoyer le cat dans le fichier 

bash-5.1$ >a>>b cat celine.txt
    -> doit renvoyer le cat dans le fichier a la suite du texte deja present

bash-5.1$ >a>>b echo voila un travail bien fait > c
    -> doit renvoyer le echo dans le fichier 'c'

bash-5.1$ cat celine.txt | rev > test.txt > a > srcs > fin | ls  > ls
bash: srcs: Is a directory





   *** TEST DES HEREDOC : ***

minishell$> grep m << EOF
> je suis maman
> tu es papa
> je suis maraine
> je suis tonton
> je suis mancho
> EOF
je suis maman
je suis maraine
je suis mancho

tester de faire une CTRL-D dans une EOF et voir si ca leak


///////////

list test : https://github.com/vladou-hopscott/minishell/wiki/Testing-commands

bash: cat << del
> $USER
> del
vnafissi

bash: cat << "del"
> $USER
> del
$USER

bash: cat << ''del
> $USER
> del
$USER

bash: cat << del
> "$USER"''
> del
"vnafissi"''

bash: cat << del
> '$USER'"$USER"
> del
'vnafissi'"vnafissi"

bash: cat << del
> $USER$USER
> del
vnafissivnafissi

bash: cat << del
> $USER1$USER
> del
vnafissi

bash: cat << del
> $USERIII
> del

bash: cat << $USER
> vnafissi
> $USER
vnafissi

bash: cat << $USER
> coucou
> $USER
coucou

bash: cat << "$USER"
> coucou
> $USER
coucou