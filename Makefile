INC=/usr/include
INCLIB=$(INC)/../lib
MLX_PATH = minilibx-linux
LIBFT_PATH = libft
MLX_MAKE = $(MAKE) -C $(MLX_PATH)
LIBFT_MAKE = $(MAKE) -C $(LIBFT_PATH)
LIBFT_LIB = ./libft/libft.a

NAME = cub3D
CC = gcc
HEADER_FILE = cub3d.h
SRCS = main.c \
	   draw_sprites.c \
	   draw_sprite_stripe.c \
	   draw_walls.c \
	   game.c \
	   mlx_utils.c \
	   key_hooks.c \
	   drawers.c \
	   utils.c \
	   free_utils.c \
	   ptrarr_utils.c \
	   vector_utils.c \
	   color_utils.c \
	   error_utils.c \
	   update_player.c \
	   initialize_player.c \
	   load_cub.c \
	   load_map.c \
	   sprite.c \
	   load_color.c \
	   load_texture.c \
	   load_resolution.c \
	   floodfill.c \
	   bmp.c \
	   debug_utils.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(MLX_PATH):
	git clone https://github.com/42Paris/minilibx-linux.git $(MLX_PATH)

$(NAME): $(MLX_PATH) ${HEADER_FILE} ${OBJS}
	$(MLX_MAKE)
	$(LIBFT_MAKE)
	$(CC) -o $(NAME) $(SRCS) $(LIBFT_LIB) -L$(MLX_PATH) -lmlx -L$(INCLIB) -lXext -lX11 -lm -lbsd

clean:
	$(MLX_MAKE) clean
	$(LIBFT_MAKE) clean
	${RM} ${OBJS}

fclean: clean
	$(LIBFT_MAKE) fclean
	${RM} ${NAME}
	${RM} -rf ${MLX_PATH}

re: fclean all

run: all
	./$(NAME) ./cubfiles/example.cub

.PHONY: all clean fclean re run
