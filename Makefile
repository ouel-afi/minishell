SRCS = path.c 

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