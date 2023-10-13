
Why when I unset it's not working ? 

void	ft_builtin_unset(char **args, t_env *env)
{
	int	i;
	int	arg_idx;

	arg_idx = 1;
	if (!args[1])
	{
		ft_putstr_fd("unset: not enough arguments\n", 2);
		return ;
	}
	while (args[arg_idx])
	{
		i = 0;
		while (env->cpy_env[i])
		{
			if (ft_strncmp(env->cpy_env[i], args[arg_idx],
				ft_strlen(args[arg_idx])) == 0
				&& env->cpy_env[i][ft_strlen(args[arg_idx])] == '=')
			{
				free(env->cpy_env[i]);
				// Shift all subsequent items up
				while (env->cpy_env[i + 1])
				{
					env->cpy_env[i] = env->cpy_env[i + 1];
					i++;
				}
				env->cpy_env[i] = NULL;
				break;
			}
			i++;
		}
		arg_idx++;
	}
}

int	ft_all_builtins(char *input)
{
	if (ft_builtin_enter(input) != 0)
		return (2);
	if (ft_builtin_write_exit(input) != 0)
	{
		free(input);
		exit(0);
	}
	if (ft_is_all_space(input) != 0)
		return (2);
	if (ft_strcmp_minishell(input, "\"\"") == 0
			|| ft_strcmp_minishell(input, "\'\'") == 0)
	{
		printf("minishell: : command not found\n");
		free(input);
		return (2);
	}
	return (0);
}

void	ft_all_builtins_verif(t_command *current, char **envp)
{
	t_env	*env_bis;
	int		cmd_count;
	char	**tab;

	cmd_count = 0;
	env_bis = (t_env *)malloc(sizeof(t_env));
	if (!env_bis)
		return ;
	while (current)
	{
		if (ft_strcmp_minishell(current->command, "pwd") == 0)
		{
			ft_builtin_pwd_fd(STDOUT_FILENO);
			exit (0);
		}
		if (ft_strcmp_minishell(current->command, "echo") == 0)
		{
			tab = parse_input_quote_echo(current->command);
			ft_builtin_echo_fd(tab);
			exit (0);
		}
		cpy_env(env_bis, envp);
		if (ft_strncmp(current->command, "unset", 5) == 0)
		{
			ft_builtin_unset(current->command_arg, env_bis);
			exit (0);
		}
		if (ft_strcmp_minishell(current->command, "env") == 0)
		{
			ft_builtin_env(env_bis);
			exit(0);
		}
		cmd_count++;
		current = current->next;
	}
}


My execve : 

minishell$> unset PATH
minishell$> env
USER=abonnefo
XDG_SESSION_TYPE=x11
SHLVL=2
HOME=/mnt/nfs/homes/abonnefo
OLDPWD=/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell
LESS=-R
DESKTOP_SESSION=ubuntu
ZSH=/mnt/nfs/homes/abonnefo/.oh-my-zsh
LSCOLORS=Gxfxcxdxbxegedabagacad
GTK_MODULES=gail:atk-bridge
XDG_SEAT_PATH=/org/freedesktop/DisplayManager/Seat0
PAGER=less
SYSTEMD_EXEC_PID=616120
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/103116/bus
LIBVIRT_DEFAULT_URI=qemu:///system
COLORTERM=truecolor
MANDATORY_PATH=/usr/share/gconf/ubuntu.mandatory.path
GTK_IM_MODULE=ibus
LOGNAME=abonnefo
_=/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell/./minishell
DEFAULTS_PATH=/usr/share/gconf/ubuntu.default.path
XDG_SESSION_CLASS=user
TERM=xterm-256color
GNOME_DESKTOP_SESSION_ID=this-is-deprecated
PATH=/mnt/nfs/homes/abonnefo/bin:/mnt/nfs/homes/abonnefo/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin
SESSION_MANAGER=local/made-f0Br6s2.clusters.42paris.fr:@/tmp/.ICE-unix/616095,unix/made-f0Br6s2.clusters.42paris.fr:/tmp/.ICE-unix/616095
GNOME_TERMINAL_SCREEN=/org/gnome/Terminal/screen/cedb5730_3bff_4693_ba33_edbac41ff0e9
XDG_MENU_PREFIX=gnome-
XDG_SESSION_PATH=/org/freedesktop/DisplayManager/Session10
XDG_RUNTIME_DIR=/run/user/103116
DISPLAY=:0
LANG=en_US.UTF-8
XDG_CURRENT_DESKTOP=Unity
XDG_SESSION_DESKTOP=ubuntu
GNOME_TERMINAL_SERVICE=:1.95
XMODIFIERS=@im=ibus
XAUTHORITY=/mnt/nfs/homes/abonnefo/.Xauthority
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
SSH_AUTH_SOCK=/run/user/103116/keyring/ssh
XDG_GREETER_DATA_DIR=/var/lib/lightdm-data/abonnefo
SSH_AGENT_LAUNCHER=gnome-keyring
KRB5CCNAME=FILE:/tmp/krb5cc_103116_0mD0zG
SHELL=/bin/zsh
QT_ACCESSIBILITY=1
GDMSESSION=ubuntu
DOCKER_HOST=unix:///run/user/103116/docker.sock
GPG_AGENT_INFO=/run/user/103116/gnupg/S.gpg-agent:0:1
QT_IM_MODULE=ibus
PWD=/mnt/nfs/homes/abonnefo/Documents/Stud-42/9-Minishell
XDG_CONFIG_DIRS=/etc/xdg/xdg-ubuntu:/etc/xdg
XDG_DATA_DIRS=/usr/share/ubuntu:/usr/share/gnome:/mnt/nfs/homes/abonnefo/.local/share/flatpak/exports/share:/var/lib/flatpak/exports/share:/usr/local/share:/usr/share:/var/lib/snapd/desktop
VTE_VERSION=6800
CHROME_DESKTOP=code-url-handler.desktop
ORIGINAL_XDG_CURRENT_DESKTOP=ubuntu:GNOME
GDK_BACKEND=x11
TERM_PROGRAM=vscode
TERM_PROGRAM_VERSION=1.82.2
GIT_ASKPASS=/usr/share/code/resources/app/extensions/git/dist/askpass.sh
VSCODE_GIT_ASKPASS_NODE=/usr/share/code/code
VSCODE_GIT_ASKPASS_EXTRA_ARGS=--ms-enable-electron-run-as-node
VSCODE_GIT_ASKPASS_MAIN=/usr/share/code/resources/app/extensions/git/dist/askpass-main.js
VSCODE_GIT_IPC_HANDLE=/run/user/103116/vscode-git-d7cd2bd933.sock
VSCODE_INJECTION=1
ZDOTDIR=/mnt/nfs/homes/abonnefo
USER_ZDOTDIR=/mnt/nfs/homes/abonnefo