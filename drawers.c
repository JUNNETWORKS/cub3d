#include "./cub3d.h"

void	clear_img(t_game *game)
{
	for (int x = 0; x < game->screen_width; x++)
	  for (int y = 0; y < game->screen_height; y++)
		my_mlx_pixel_put(game, x, y, 0x00000000);
}

void	draw_block(t_game *game, int x, int y, int color)
{
	// 正方形を描画
	for (int xx = 0; xx < BLOCK_SIZE; xx++){
		for (int yy = 0; yy < BLOCK_SIZE; yy++){
			my_mlx_pixel_put(game, x + xx, y + yy, color);
		}
	}
}

// 2つのベクトル間に直線を引く
// ブレゼンハムのアルゴリズム
void	draw_2vec2(t_game *game, t_vec2 v1, t_vec2 v2, int color)
{
	int v1x = (int)v1.x;
	int v1y = (int)v1.y;
	int v2x = (int)v2.x;
	int v2y = (int)v2.y;
	int dx = ABS(v2x - v1x);
	int dy = ABS(v2y - v1y);
	if (dx == 0 && dy == 0)
		return;
	int sx = (v1x < v2x) ? 1 : -1;  // Eが閾値を超えた時増加量
	int sy = (v1y < v2y) ? 1 : -1;
	int dx2 = dx * 2;
	int dy2 = dy * 2;

	int x = v1.x;
	int y = v1.y;
	if (dy <= dx) {
		int E = -dx;
		for (int i = 0; i <= dx; i++) {
			if (x >= 0 && x < game->screen_width && y >= 0 && y < game->screen_height)
				my_mlx_pixel_put(game, x, y, color);
			x += sx;
			E += dy2;
			if (0 <= E) {
				y += sy;
				E -= dx2;
			}
		}
	} else {
		int E = -dy;
		for (int i = 0; i <= dy; i++) {
			if (x >= 0 && x < game->screen_width && y >= 0 && y < game->screen_height)
				my_mlx_pixel_put(game, x, y, color);
			y += sy;
			E += dx2;
			if (0 <= E) {
				x += sx;
				E -= dy2;
			}
		}
	}
}

// position: 位置ベクトル
// angle: 角度(rad)
// length: 線の長さ
void	draw_line_angle_length(t_game *game, t_vec2 position, double angle, int length, int color)
{
	/* 長さ分の線を描画 */
	for(int l = 0; l < length; l++){
		/* x座標とy座標を計算 */
		int x = position.x + l * cos(angle);
		int y = position.y + l * sin(angle);
		if (x < 0 || x >= game->screen_width || y < 0 || y >= game->screen_height)
			continue;
		my_mlx_pixel_put(game, x, y, color);
	}
}

