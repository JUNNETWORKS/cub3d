#ifndef CUB3D_H
# define CUB3D_H
# include "./minilibx-linux/mlx.h"
# include "./libft/libft.h"
# include <X11/X.h>
# include <stdio.h>
# include <stdint.h>
# include <math.h>

// Key definitions
# define KEY_q 113
# define KEY_esc 65307

// 描画するためのイメージ情報を保持
typedef struct  s_img {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_img;

// mlxのポインタやウィンドウのポインタを保持
typedef struct  s_game {
    void		*mlx;
    void		*win;
	t_img		img;
}               t_game;

// MLX Utils
void            my_mlx_pixel_put(t_game *game, int x, int y, int color);

#endif
