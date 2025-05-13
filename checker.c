/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:14:45 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/05/12 20:16:36 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_parenthesis(t_token *token)
{
	int		paren;
	t_token	*tmp;

	paren = 0;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == 9)
			paren++;
		else if (tmp->type == 10)
			paren--;
		tmp = tmp->next;
	}
	if (paren != 0)
	{
		ft_putstr_fd("bash : syntax unmatched parenthesis\n", 2);
		return (paren);
	}
	return (0);
}

int	check_errors(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	if (tmp->type == 2 || tmp->type == 11
		|| tmp->type == 12)
	{
		printf("bash: syntax error near unexpected token `%s'\n", token->value);
		return (1);
	}
	while (tmp)
	{
		if ((tmp->type == 2 || tmp->type == 5 
				|| tmp->type == 6 || tmp->type == 7 
				|| tmp->type == 8 || tmp->type == 9 
				|| tmp->type == 11 || tmp->type == 12) && !tmp->next)
		{
			printf("bash: syntax error near unexpected token `newline'\n");
			return (1);
		}
		if (tmp->type != 1 && tmp->type != 3
			&& tmp->type != 4 && tmp->type != 10
			&& tmp->next && tmp->next->type == 10)
		{
			printf("bash: syntax error near unexpected token `)'\n");
			return (1);
		}
		if ((tmp->type == 2 || tmp->type == 5 
				|| tmp->type == 6 || tmp->type == 7 
				|| tmp->type == 8 || tmp->type == 11 
				|| tmp->type == 12)
			&& (tmp->next->type == 2 || tmp->next->type == 5
				|| tmp->next->type == 6 || tmp->next->type == 7
				|| tmp->next->type == 8 || tmp->next->type == 11
				|| tmp->next->type == 12 || tmp->next->value[0] == '&'))
		{
			printf("bash: syntax error near unexpected token `%s'\n",
				tmp->next->value);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
