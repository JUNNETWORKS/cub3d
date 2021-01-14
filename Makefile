INC=/usr/include
INCLIB=$(INC)/../lib
MLX_PATH = minilibx-linux
NAME = cub3d
CC = gcc
SRCS = main.c
OBJS = $(SRCS:.c=.o)
MLX_MAKE = $(MAKE) -C $(MLX_PATH)

all: $(NAME)

$(NAME): ${OBJS}
	${MLX_MAKE}
	$(CC) -o $(NAME) $(OBJS) -L$(MLX_PATH) -lmlx -L$(INCLIB) -lXext -lX11 -lm -lbsd

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
