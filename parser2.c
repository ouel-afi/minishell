/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:37:02 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/05/12 21:12:06 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_cmd(t_token *token)
{
	t_token	*tmp;
	int		count;

	tmp = token;
	count = 0;
	while (tmp)
	{
		if (tmp->type == 1 || tmp->type == 3
			|| tmp->type == 4)
			count++;
		if (tmp->type == 5 || tmp->type == 6
			|| tmp->type == 7 || tmp->type == 8)
			tmp = tmp->next;
		tmp = tmp->next;
	}
	return (count);
}

t_tree	*parse_cmd(t_token *token)
{
	t_token	*redir_tail;
	t_token	*new_redir;
	t_token	*redir;
	int		last_type;
	t_token	*tmp;
	int		nb_cmd;
	char	**cmd_args;
	int		i;

	redir_tail = NULL;
	new_redir = NULL;
	redir = NULL;
	last_type = 0;
	tmp = token;
	nb_cmd = calculate_cmd(tmp);
	cmd_args = malloc(sizeof(char *) * (nb_cmd + 1));
	i = 0;
	if (!cmd_args)
		return (NULL);
	while (tmp)
	{
		if (tmp->type == 9 || tmp->type == 10)
		{
			printf("bash : syntax error parenthesis\n");
			return (NULL);
		}
		if (tmp->type == 1 || tmp->type == 3 || tmp->type == 4)
		{
			cmd_args[i] = ft_strdup(tmp->value);
			i++;
			tmp = tmp->next;
		}
		else if (tmp->next)
		{
			last_type = tmp->type;
			tmp = tmp->next;
			new_redir = create_token(tmp->value, 0, 0);
			new_redir->type = last_type;
			new_redir->next = NULL;
			if (!redir)
			{
				redir = new_redir;
				redir_tail = redir;
			}
			else
			{
				redir_tail->next = new_redir;
				redir_tail = new_redir;
			}
			tmp = tmp->next;
		}
	}
	cmd_args[i] = NULL;
	return (create_tree_node(token, cmd_args, redir));
}
