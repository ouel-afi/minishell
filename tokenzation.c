/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenzation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 15:24:04 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/04/06 11:43:16 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*initialize_lexer(char *input)
{
	t_lexer	*lexer;

	lexer = malloc((sizeof(t_lexer)));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->lenght = ft_strlen(input);
	lexer->position = 0;
	return (lexer);
}

int	is_space(t_lexer *lexer)
{
	if ((lexer->input[lexer->position] >= 9 
			&& lexer->input[lexer->position] <= 13)
		|| lexer->input[lexer->position] == 32)
		return (1);
	return (0);
}

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->position < lexer->lenght && is_space(lexer))
		lexer->position++;
}

t_token	*create_token(char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	else
	{
		token->value = ft_strdup(value);
		token->next = NULL;
	}
	return (token);
}

t_token	*handle_quote(t_lexer *lexer, char quote)
{
	size_t	lenght;
	size_t	start;
	char	*value;

	lexer->position += 1;
	start = lexer->position;
	while (lexer->position < lexer->lenght 
		&& lexer->input[lexer->position] != quote)
		lexer->position++;
	if (lexer->position >= lexer->lenght)
	{
		perror("unclosed quote\n");
		return (NULL);
	}
	lenght = lexer->position - start;
	value = ft_substr(lexer->input, start, lenght);
	lexer->position += 1;
	return (create_token(value));
}

t_token	*handle_operations(t_lexer *lexer, char *oper, int i)
{
	char	*str;

	str = ft_substr(oper, 0, i);
	if (!str)
		return (NULL);
	str[i] = '\0';
	lexer->position += i;
	return (create_token(str));
}

// t_token	*handle_others(t_lexer *lexer, char *oper)
// {
// 	char *str;

// 	str = ft_substr(oper, 0, 2);
// 	if(!str)
// 		return NULL;
// 	str[2] = '\0';
// 	lexer->position += 2;
// 	return (create_token(str));
// }

t_token	*handle_word(t_lexer *lexer)
{
	char	*word;
	size_t	start;
	size_t	lenght;

	start = lexer->position;
	while (lexer->position < lexer->lenght && (!is_space(lexer)) 
		&& lexer->input[lexer->position] != '\'' 
		&& lexer->input[lexer->position] != '"' 
		&& lexer->input[lexer->position] != '|' 
		&& lexer->input[lexer->position] != '<' 
		&& lexer->input[lexer->position] != '>' 
		&& lexer->input[lexer->position] != '(' 
		&& lexer->input[lexer->position] != ')'
		&& lexer->input[lexer->position] != '&')
		lexer->position++;
	lenght = lexer->position - start;
	word = ft_substr(lexer->input, start, lenght);
	return (create_token(word));
}

t_token	*get_next_token(t_lexer *lexer)
{
	char	*current;

	skip_whitespace(lexer);
	if (lexer->position >= lexer->lenght)
		return (NULL);
	current = lexer->input + lexer->position;
	if (current[0] == '\'' || current[0] == '"')
		return (handle_quote(lexer, *current));
	if ((lexer->input[lexer->position] == '|' 
			&& lexer->input[lexer->position + 1] == '|') 
		|| (lexer->input[lexer->position] == '&' 
			&& lexer->input[lexer->position + 1] == '&'))
		return (handle_operations(lexer, current, 2));
	if ((lexer->input[lexer->position] == '>' 
			&& lexer->input[lexer->position + 1] == '>') 
		|| (lexer->input[lexer->position] == '<' 
			&& lexer->input[lexer->position + 1] == '<'))
		return (handle_operations(lexer, current, 2));
	if (current[0] == '|' || current[0] == '<' || current[0] == '>' 
		|| current[0] == '(' || current[0] == ')')
		return (handle_operations(lexer, current, 1));
	return (handle_word(lexer));
}

int	cmd_type(t_token *token, int first_cmd)
{
	if (first_cmd == 0)
	{
		first_cmd++;
		return (1);
	}
	else if (token->type == PIPE || token->type == OPEN_PAREN 
		|| token->type == DOUBLE_QUOTE || token->type == OR 
		|| token->type == AND)
		return (1);
	else
		return (0);
}

t_type	token_type(t_token *token)
{
	if (strcmp(token->value, "\"") == 0)
		return (SINGLE_QUOTE);
	else if (strcmp(token->value, "'") == 0)
		return (DOUBLE_QUOTE);
	else if (strcmp(token->value, "||") == 0)
		return (OR);
	else if (strcmp(token->value, "&&") == 0)
		return (AND);
	else if (strcmp(token->value, "|") == 0)
		return (PIPE);
	else if (strcmp(token->value, ">>") == 0)
		return (APPEND);
	else if (strcmp(token->value, "<<") == 0)
		return (HEREDOC);
	else if (strcmp(token->value, "<") == 0)
		return (REDIR_IN);
	else if (strcmp(token->value, ">") == 0)
		return (REDIR_OUT);
	else if (strcmp(token->value, "(") == 0)
		return (OPEN_PAREN);
	else if (strcmp(token->value, ")") == 0)
		return (CLOSE_PAREN);
	else
		return (WORD);
}

int	main(int argc, char **argv)
{
	char	*input;
	t_lexer	*lexer;
	t_token	*token;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("minishell> ");
		lexer = initialize_lexer(input);
		while (lexer->position < lexer->lenght)
		{
			token = get_next_token(lexer);
			if (!token->value)
				return (0);
			token->type = token_type(token);
		}
	}
	return (0);
}
