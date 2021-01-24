INC=/usr/include
INCLIB=$(INC)/../lib
MLX_PATH = minilibx-linux
LIBFT_PATH = libft
MLX_MAKE = $(MAKE) -C $(MLX_PATH)
LIBFT_MAKE = $(MAKE) -C $(LIBFT_PATH)
LIBFT_LIB = ./libft/libft.a

NAME = cub3d
CC = gcc
SRCS = main.c \
	   mlx_utils.c \
	   key_hooks.c \
	   drawers.c \
	   utils.c \
	   vector_utils.c \
	   debug_utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): ${OBJS}
	$(MLX_MAKE)
	$(LIBFT_MAKE)
	$(CC) -g -fsanitize=address -o $(NAME) $(SRCS) $(LIBFT_LIB) -L$(MLX_PATH) -lmlx -L$(INCLIB) -lXext -lX11 -lm -lbsd

clean:
	$(MLX_MAKE) clean
	$(LIBFT_MAKE) clean
	${RM} ${OBJS}

fclean: clean
	$(LIBFT_MAKE) fclean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
