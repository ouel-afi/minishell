#include "minishell.h"

void write_split(char **split)
{
	int i = 0;
	while (split[i])
	{
		printf("%s\n", split[i]);
		i++;
	}
}

char **find_path(char **env)
{
	int i = 0;
	// char *env[] = {"PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/usr/local/munki", NULL};
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return(ft_split(env[i] + 5, ':'));
		i++;
	}
	return NULL;
}

char *check_path(char **env, char *cmd)
{
	char **path = find_path(env);
	int i = 0;
	char *full_path;
	while(path[i])
	{
		if (cmd[0] == '/')
		{
			if (access(cmd, F_OK | X_OK) == 0)
			{
				write(1, "access\n", 7);
				return (ft_strdup(cmd));
			}
		}
		path[i] = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(path[i], cmd);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			return(ft_strdup(full_path));
		}
		i++;
	}
	return NULL;
}

// void	child(char **cmd)
// {
	// printf("%s\n", *cmd);
// 	execve("/bin/ls", cmd, NULL);
// }

void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int main(int ac, char **av, char **env)
{

	(void)ac;
	(void)av;
	t_node *main = NULL;
	pid_t pid;
	char **cmd;
	char *ret;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler);
	rl_catch_signals = 0;
	char *str = "minishell> ";
	while(1)
	{
		ret = readline(str);
		if (!ret)
		{
			write(1,"exit\n", 5);
			exit(0);
		}
		else if (ret)
		{
			pid  = fork();
			cmd = ft_split(ret, 32);
			if (pid == 0)
			{
			if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/'))
			{
				if (access(*cmd, F_OK | X_OK) == 0)
				{
					execve(cmd[0], cmd, NULL);
		
				}
			}
				char *full_path = check_path(env, cmd[0]);
				execve(full_path, cmd, env);
			}
			wait(NULL);
		}
	}
}
