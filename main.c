/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ouel-afi <ouel-afi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 14:31:55 by taya              #+#    #+#             */
/*   Updated: 2025/07/07 13:19:24 by ouel-afi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void setup_shell_terminal(void)
{
    pid_t shell_pgid;

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    shell_pgid = getpid();
    if (setpgid(shell_pgid, shell_pgid) < 0)
    {
        perror("Couldn't put the shell in its own process group");
        exit(1);
    }
    if (tcsetpgrp(STDIN_FILENO, shell_pgid) < 0)
    {
        perror("Couldn't set terminal control");
        exit(1);
    }
}
#include <termios.h>

void    reset_terminal_mode(void)
{
    struct termios    term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    write(STDERR_FILENO, "\r\033[K", 4);
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHOCTL);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void vv()
{
    system("leaks minishell");
}

// int main(int argc, char **argv, char **env)
// {
//     char *input;
//     t_lexer *lexer;
//     t_token *token;
//     t_token *token_list;
//     t_tree *node;
//     t_env *envlist;
// 		int last_exit_status = 0;
// 	(void)argc;
// 	(void)argv;
//     input = NULL;
//     lexer = NULL;
//     node = NULL;
//     token_list = NULL;
//     // atexit(vv);
//     envlist = init_env(env);
//     setup_shell_terminal();
//     signal(SIGQUIT, SIG_IGN);
//     signal(SIGINT, handler);
//     rl_catch_signals = 0;
    
//     while (1)
//     {
//         input = readline("minishell> ");
//         if (!input)
//         {
//             write(1, "exit\n", 5);
//             break;
//         }
//         if (input[0] == '\0')
//         {
//             free(input);
//             continue;
//         }  
//         add_history(input);
//         token_list = NULL;
//         lexer = initialize_lexer(input);
// 		while (lexer->position < lexer->lenght)
// 		{
// 			token = get_next_token(lexer);
// 			if (!token)
// 			    continue;
// 			token->type = token_type(token);
// 			append_token(&token_list, token);
// 		}
//         if (!token_list)
//         {
//             free(input);
// 			continue ;
//         }
// 		if (check_parenthesis(token_list) != 0)
// 			continue;
//         merge_tokens(&token_list);
//         if (check_errors(token_list) == 1)
// 			continue;
//         node = parse_op(token_list);
//         if(!node)
// 			continue;
//         process_heredocs_tree(node);
//         last_exit_status = execute_tree(node, &envlist, last_exit_status);  
//         free(input);
//     }
//     free_env_list(envlist);
//     return (0);
// }

int calculate_dolalr_sign(char *input)
{
	int i = 0;
	int dollar_sign = 0;
	while (input[i])
	{
		if ((input[i] == '$' && input[i + 1] == '$') || (input[i] == '$' && input[i + 1] == '\0'))
		{
			dollar_sign++;
			i++;
			if (input[i] == '\0')
				break;
		}
		else if (input[i] == '$')
			dollar_sign++;
		i++;
	}
	return (dollar_sign);
}

int *is_expand(t_token *token, int dollar_sign)
{
	int j = 0;
	int i = 0;
	int *expand = malloc(sizeof(int) * dollar_sign);
	t_token *tmp = token;
	char *input;
	while (tmp)
	{
		input = tmp->value;
		if (tmp->type == 1 || tmp->type == 3 || tmp->type == 4)
		{
			while (input[i])
			{	
				if ((input[i] == '$' && input[i + 1] == '$') || (input[i] == '$' && input[i + 1] == '\0'))
				{
					expand[j] = 2;
					// if (input[i + 1] == '$' && tmp->type == 3)
					// 	expand[j] = 3;
					j++;
					i++;
					if (input[i] == '\0')
						break;
				}
				else if (input[i] == '$')
				{
					expand[j] = 1;
					if (tmp->type == 3)
						expand[j] = 0;
					j++;	
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (expand);
}

int main(int argc, char **argv, char **env)
{
	int dollar_sign = 0;
    char *input;
	int *to_expand;
    t_lexer *lexer;
    t_token *token;
    t_token *token_list;
    t_tree *node;
    t_env *envlist;
    int last_exit_status = 0;
	// atexit(vv);
    
    (void)argc;
    (void)argv;
    input = NULL;
    lexer = NULL;
    node = NULL;
    token_list = NULL;
    
    envlist = init_env(env);
    setup_shell_terminal();
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, handler);
    rl_catch_signals = 0;
    
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
        {
            write(1, "exit\n", 5);
            break;
        }
        
        if (input[0] == '\0')
        {
            free(input);
            continue;
        }
        // dollar_sign = calculate_dollar_sign(input);
		add_history(input);
        token_list = NULL;
        lexer = initialize_lexer(input);
        while (lexer->position < lexer->lenght)
        {
            token = get_next_token(lexer);
            if (!token)
                continue;
            token->type = token_type(token);
            append_token(&token_list, token);
        }
		to_expand  = ft_strdup(is_expand(token_list, dollar_sign));
        free_lexer(lexer);
        lexer = NULL; 
        if (!token_list)
        {
            free(input);
            continue;
        }
        
        if (check_parenthesis(token_list) != 0)
        {
            free(input);
            free_token_list(token_list);
            continue;
        }
        
        if (merge_tokens(&token_list))
            free_token_list(token);
        
        if (check_errors(token_list) == 1)
        {
            free(input);
            free_token_list(token_list);
            continue;
        }
        
        node = parse_op(token_list);
        if (!node)
        {
            free(input);
            free_token_list(token_list); 
            continue;
        }   
        process_heredocs_tree(node);
        last_exit_status = execute_tree(node, &envlist, last_exit_status, to_expand);
        free(input);
        free_token_list(token_list);
        free_tree(node);      
    }
    free_env_list(envlist);
    rl_clear_history(); 
    
    return (0);
}