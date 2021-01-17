#include "cub3d.h"

// mlx_pixel_put() はレンダリングを待たずに1ピクセルずつ描画するので激遅, なので同じ動きをする関数を自作する
void            my_mlx_pixel_put(t_game *game, int x, int y, int color)
{
    char    *dst;

	// line_lengthは実際のウィンドウの横幅と違うので計算する必要がある
    dst = game->img.addr + (y * game->img.line_length + x * (game->img.bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}
