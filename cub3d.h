#ifndef CUB3D_H
# define CUB3D_H
# include "./minilibx-linux/mlx.h"
# include "./libft/libft.h"
# include <X11/X.h>
# include <stdio.h>
# include <stdint.h>
# include <math.h>

// display definitions
# define WIDTH 1920
# define HEIGHT 1080
# define BLOCK_SIZE 20

// Key definitions
# define KEY_q 113
# define KEY_esc 65307

// 描画するためのイメージ情報を保持
typedef struct	s_img {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_img;

// Player
typedef struct	s_player {
	int			*position;
}				t_player;

// mlxのポインタやウィンドウのポインタを保持
typedef struct  s_game {
	void		*mlx;
	void		*win;
	t_img		img;
	t_player	player;
}               t_game;

// MLX Utils
void			my_mlx_pixel_put(t_game *game, int x, int y, int color);
// Game functions
void			initialize_game(t_game *game);
// Hooks
int 			key_hook(int keycode, t_game *game);

#endif
