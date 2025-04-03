/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:21:37 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/04/03 15:27:02 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(char **cmd)
{
	// printf("%s\n", *cmd);
	execve("/bin/ls", cmd, NULL);
}

void write_split(char **split)
{
	int i = 0;
	while (split[i])
	{
		printf("%s\n", split[i]);
		i++;
	}
}

int main()
{
	pid_t pid;
	char **cmd;
	char *ret;
	char *str = "minishell> ";
	while(1)
	{
		ret = readline(str);
		if (ret)
		{
			cmd = ft_split(ret, 32);
			pid  = fork();
			if (pid == 0)
				child(cmd);
			else if (pid > 0)
				wait(NULL);
		}
		if (!ret)
			break ;
	}
}
