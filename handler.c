/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 20:09:13 by ouel-afi          #+#    #+#             */
/*   Updated: 2025/05/12 22:57:18 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		ft_putstr_fd("bash : syntax error unclosed quote\n", 2);
		return (0);
	}
	lenght = lexer->position - start;
	value = ft_substr(lexer->input, start, lenght);
	lexer->position += 1;
	if (lexer->input[lexer->position] == 32)
		return (create_token(value, quote, 1));
	return (create_token(value, quote, 0));
}

t_token	*handle_operations(t_lexer *lexer, char *oper, int i)
{
	char	*str;

	str = ft_substr(oper, 0, i);
	if (!str)
		return (NULL);
	str[i] = '\0';
	lexer->position += i;
	if (lexer->input[lexer->position] == 32)
		return (create_token(str, str[0], 1));
	return (create_token(str, str[0], 0));
}

t_token	*handle_word(t_lexer *lexer)
{
	write(1, "1\n", 2);
	char	*result;
	char	*temp;
	size_t	start;
	int		has_space;
	int		in_quotes;
	char	quote;
	char	*new_result;

	result = ft_strdup("");
	temp = NULL;
	has_space = 0;
	in_quotes = 0;
	while (lexer->position < lexer->lenght && !is_space(lexer))
	{
		write(1, "2\n", 2);
		if (lexer->input[lexer->position] == '\''
			|| lexer->input[lexer->position] == '"')
		{
			write(1, "3\n", 2);
			printf("%c\n", lexer->input[lexer->position]);
			quote = lexer->input[lexer->position++];
			start = lexer->position;
			in_quotes = 1;
			while (lexer->position < lexer->lenght
				&&lexer->input[lexer->position] != quote)
				lexer->position++;
			if (lexer->position >= lexer->lenght)
			{
				ft_putstr_fd("bash : syntax error near unexpected token `)'\n", 2);
				// free(result);
				return (0);
			}
			temp = ft_substr(lexer->input, start, lexer->position - start);
			new_result = ft_strjoin(result, temp);
			// free(result);
			// free(temp);
			result = new_result;
			lexer->position++;
			in_quotes = 0;
		}
		else if (!in_quotes && ft_strchr("|<>()&", lexer->input[lexer->position])){
			write(1, "4\n", 2);
			break ;
		}
		else
		{
			write(1, "5\n", 2);
			if (in_quotes){
				write(1, "6\n", 2);
				break ;
			}
			printf("%c\n", lexer->input[lexer->position]);
			start = lexer->position;
			while (lexer->position < lexer->lenght
				&& !ft_strchr("\"'|<>()& ", lexer->input[lexer->position]))
				lexer->position++;
			temp = ft_substr(lexer->input, start, lexer->position - start);
			new_result = ft_strjoin(result, temp);
			// free(result);
			// free(temp);
			result = new_result;
		}
	}
	has_space = (lexer->position < lexer->lenght
			&& lexer->input[lexer->position] == ' ');
	return (create_token(result, in_quotes ? 4 : 1, has_space));
}

// int	is_special_char(char c)
// {
// 	return (ft_strchr("|<>()&", c) != NULL);
// }

// int	is_quote(char c)
// {
// 	return (c == '\'' || c == '"');
// }

// void	handle_word_quote(char **result, t_lexer *lexer, int *in_quotes)
// {
// 	char	quote;
// 	char	*temp;
// 	char	*new_result;
// 	size_t	start;

// 	quote = lexer->input[lexer->position++];
// 	start = lexer->position;
// 	*in_quotes = 1;
// 	while (lexer->position < lexer->lenght
// 		&& lexer->input[lexer->position] != quote)
// 		lexer->position++;
// 	if (lexer->position >= lexer->lenght)
// 	{
// 		ft_putstr_fd("bash : syntax error near unexpected token `)'\n", 2);
// 		return ;
// 	}
// 	temp = ft_substr(lexer->input, start, lexer->position - start);
// 	new_result = ft_strjoin(*result, temp);
// 	*result = new_result;
// 	lexer->position++;
// 	*in_quotes = 0;
// }

// void	handle_plain_word(char **result, t_lexer *lexer)
// {
// 	size_t	start;
// 	char	*temp;
// 	char	*new_result;

// 	start = lexer->position;
// 	while (lexer->position < lexer->lenght
// 		&& !ft_strchr("\"'|<>()& ", lexer->input[lexer->position]))
// 		lexer->position++;
// 	temp = ft_substr(lexer->input, start, lexer->position - start);
// 	new_result = ft_strjoin(*result, temp);
// 	*result = new_result;
// }

// t_token	*handle_word(t_lexer *lexer)
// {
// 	char	*result;
// 	int		has_space;
// 	int		in_quotes;
// 	int		type;

// 	result = ft_strdup("");
// 	in_quotes = 0;
// 	while (lexer->position < lexer->lenght && !is_space(lexer))
// 	{
// 		if (is_quote(lexer->input[lexer->position]))
// 			handle_word_quote(&result, lexer, &in_quotes);
// 		else if (!in_quotes && is_special_char(lexer->input[lexer->position]))
// 			break ;
// 		else
// 			handle_plain_word(&result, lexer);
// 	}
// 	if (lexer->position < lexer->lenght && lexer->input[lexer->position] == ' ')
// 		has_space = 1;
// 	else
// 		has_space = 0;
// 	if (in_quotes)
// 		type = 4;
// 	else
// 		type = 1;
// 	return (create_token(result, type, has_space));
// }
