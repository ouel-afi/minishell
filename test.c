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
	if (!path)
		return NULL;
		
	int i = 0;
	char *full_path = NULL;
	char *with_slash = NULL;
	
	while(path[i])
	{
		with_slash = ft_strjoin(path[i], "/");
		full_path = ft_strjoin(with_slash, cmd);
		free(with_slash);  // Free the intermediate string
		
		if (access(full_path, F_OK | X_OK) == 0)
		{
			// Free the path array before returning
			int j = 0;
			while (path[j])
				free(path[j++]);
			free(path);
			return full_path;  // Caller will need to free this
		}
		
		free(full_path);  // Free this attempt
		i++;
	}
	
	// Free the path array
	i = 0;
	while (path[i])
		free(path[i++]);
	free(path);
	
	return NULL;
}

void	child(char **cmd)
{
	// printf("%s\n", *cmd);
	execve("/bin/ls", cmd, NULL);
}

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
		else if (ret[0] != '\0')  // Only process non-empty commands
		{
			add_history(ret);  // Add command to history
			
			cmd = ft_split(ret, 32);
			if (cmd[0] != NULL)  // Check if there's a command
			{
				pid = fork();
				if (pid == 0)
				{
					// Handle absolute paths
					if (ret[0] == '/' || (ret[0] == '.' && ret[1] == '/'))
					{
						if (access(cmd[0], F_OK | X_OK) == 0)
							execve(cmd[0], cmd, env);
					}
					else
					{
						char *full_path = check_path(env, cmd[0]);
						if (full_path)
						{
							execve(full_path, cmd, env);
							free(full_path);  // This won't execute if execve succeeds
						}
						else
							fprintf(stderr, "Command not found: %s\n", cmd[0]);
					}
					// If we get here, execve failed
					exit(EXIT_FAILURE);
				}
				else if (pid < 0)
				{
					perror("fork");
				}
				wait(NULL);
				
				// Free command array
				int i = 0;
				while (cmd[i])
				{
					free(cmd[i]);
					i++;
				}
				free(cmd);
			}
			free(ret);  // Free the readline result
		}
	}
}