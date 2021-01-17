#ifndef CUB3D_H
# define CUB3D_H
# include "./minilibx-linux/mlx.h"
# include "./libft/libft.h"
# include <X11/X.h>
# include <stdio.h>
# include <stdint.h>
# include <math.h>

// mlxのポインタやウィンドウのポインタを保持
typedef struct  s_vars {
    void        *mlx;
    void        *win;
}               t_vars;

// 描画するためのイメージ情報を保持
typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

// MLX Utils
void            my_mlx_pixel_put(t_data *data, int x, int y, int color);

#endif
