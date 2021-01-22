#include "./cub3d.h"

void	clear_img(t_game *game)
{
	for (int x = 0; x < WIDTH; x++)
	  for (int y = 0; y < HEIGHT; y++)
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
	int dx = ABS(v2.x - v1.x);
	int dy = ABS(v2.y - v1.y);
	if (dx == 0 && dy == 0)
		return;
	int sx = (v1.x < v2.x) ? 1 : -1;  // Eが閾値を超えた時増加量
	int sy = (v1.y < v2.y) ? 1 : -1;
	int dx2 = dx * 2;
	int dy2 = dy * 2;

	int x = v1.x;
	int y = v1.y;
	if (dy <= dx) {
		int E = -dx;
		for (int i = 0; i <= dx; i++) {
			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
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
			if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
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
// angle: 角度(deg)
// length: 線の長さ
void	draw_line_angle_length(t_game *game, t_vec2 position, int angle, int length, int color)
{
	double rad = (double)angle / 180 * M_PI;
	/* 長さ分の線を描画 */
	for(int l = 0; l < length; l++){
		/* x座標とy座標を計算 */
		int x = position.x + l * cos(rad);
		int y = position.y + l * sin(rad);
		if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
			continue;
		my_mlx_pixel_put(game, x, y, color);
	}
}

