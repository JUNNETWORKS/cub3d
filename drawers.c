#include "./cub3d.h"

void	clear_img(t_img *img)
{
	for (int x = 0; x < img->width; x++)
	  for (int y = 0; y < img->height; y++)
		my_mlx_pixel_put(img, x, y, 0xff000000);
}

void	draw_block(t_img *img, int x, int y, int color)
{
	// 正方形を描画
	for (int xx = 0; xx < BLOCK_SIZE; xx++){
		for (int yy = 0; yy < BLOCK_SIZE; yy++){
			my_mlx_pixel_put(img, x + xx, y + yy, color);
		}
	}
}

// 2つのベクトル間に直線を引く
// ブレゼンハムのアルゴリズム
void	draw_2vec2(t_img *img, t_vec2 v1, t_vec2 v2, int color, int thickness)
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
			for (int t = -thickness / 2; t < thickness / 2; t++){
				if (x >= 0 && x < img->width && y+t >= 0 && y+t < img->height)
					my_mlx_pixel_put(img, x, y+t, color);
			}
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
			for (int t = -thickness / 2; t < thickness / 2; t++){
				if (x+t >= 0 && x+t < img->width && y >= 0 && y < img->height)
					my_mlx_pixel_put(img, x+t, y, color);
			}
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
void	draw_line_angle_length(t_img *img, t_vec2 position, double angle, int length, int color)
{
	/* 長さ分の線を描画 */
	for(int l = 0; l < length; l++){
		/* x座標とy座標を計算 */
		int x = position.x + l * cos(angle);
		int y = position.y + l * sin(angle);
		if (x < 0 || x >= img->width || y < 0 || y >= img->height)
			continue;
		my_mlx_pixel_put(img, x, y, color);
	}
}

