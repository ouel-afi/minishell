# SRCS  = tokenzation.c exec.c
# CC = cc
# CFLAGS = -Wall -Wextra -Werror
# NAME = minishell

# OBJS = $(SRCS:.c=.o)

# LIBFT_DIR = ./libft
# LIBFT = $(LIBFT_DIR)/libft.a

# all: $(NAME)

# $(NAME): $(OBJS) $(LIBFT)
# 	$(CC) $(CFLAGS) -L/Users/ouel-afi/goinfre/homebrew/opt/readline/lib -lreadline $(OBJS) $(LIBFT) -o $(NAME)

# $(LIBFT):
# 	$(MAKE) -C $(LIBFT_DIR)

# %.o: %.c 
# 	$(CC) $(CFLAGS) -I/Users/ouel-afi/goinfre/homebrew/opt/readline/include -c $< -o $@

# clean:
# 	rm -f $(OBJS) 
# 	$(MAKE) clean -C $(LIBFT_DIR)

# fclean: clean
# 	rm -f $(NAME)
# 	$(MAKE) fclean -C $(LIBFT_DIR)

# re: fclean all

# .PHONY: all bonus clean fclean re







SRCS =  main.c lexer.c tokenizer.c parser.c parser2.c create_tree.c checker.c checker2.c handler.c handle_word.c print.c

CC = cc
CFLAGS = -Wall -Wextra -Werror 
NAME = minishell

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -L/Users/ouel-afi/goinfre/homebrew/opt/readline/lib -lreadline $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

%.o: %.c 
	$(CC) $(CFLAGS) -I/Users/ouel-afi/goinfre/homebrew/opt/readline/include -c $< -o $@

clean:
	rm -f $(OBJS) 
	$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all bonus clean fclean re






# SRCS = exec.c builtin.c main.c test.c  tree.c check_path.c
# CC = cc
# CFLAGS = -Wall -Wextra -Werror
# NAME = minishell

# OBJS = $(SRCS:.c=.o)

# LIBFT_DIR = ./libft
# LIBFT = $(LIBFT_DIR)/libft.a

# all: $(NAME)

# $(NAME):	$(OBJS)	$(LIBFT)
# 	$(CC)	$(CFLAGS) -lreadline -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include $(OBJS) $(LIBFT) -o $(NAME)

# $(LIBFT):
# 	$(MAKE) -C $(LIBFT_DIR)

# %.o: %.c 
# 	$(CC) $(CFLAGS) -I/opt/homebrew/opt/readline/include -c $< -o $@

# clean:
# 	rm -f $(OBJS) 
# 	$(MAKE) clean -C $(LIBFT_DIR)

# fclean: clean
# 	rm -f $(NAME)
# 	$(MAKE) fclean -C $(LIBFT_DIR)

# re: fclean all

# .PHONY: all bonus clean fclean re