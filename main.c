/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 13:21:37 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/04/03 13:29:11 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac , char **av)
{
	char *str = "minishell> ";
	while(1)
	{
		char *ret = readline(str);
		if (ret)
		{
			printf("\n");
		}
		if (!ret)
			break ;
	}
}
