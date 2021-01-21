#include "cub3d.h"

char *MAP[] = {
	"1111111111111111111111111",
	"1000000000110000000000001",
	"1011000001110000002000001",
	"100100000000000000000000111111111",
	"111111111011000001110000000000001",
	"100000000011000001110111110111111",
	"11110111111111011100000010001",
	"11110111111111011101010010001",
	"11000000110101011100000010001",
	"10002000000000001100000010001",
	"10000000000000001101010010001",
	"11000001110101011101011010N0111",
	"11110111 1110101 101111010001",
	"11111111 1111111 111111111111",
};

double	deg2rad(int x)
{
	return ((double)x / 180 * M_PI);
}

void	clear_img(t_game *game)
{
	for (int x = 0; x < WIDTH; x++)
	  for (int y = 0; y < HEIGHT; y++)
		my_mlx_pixel_put(game, x, y, 0x00000000);
}

void	draw_block(t_game *game, int x, int y, int color){
	// 正方形を描画
	for (int xx = 0; xx < BLOCK_SIZE; xx++){
		for (int yy = 0; yy < BLOCK_SIZE; yy++){
			my_mlx_pixel_put(game, x + xx, y + yy, color);
		}
	}
}

// 2つのベクトル間に直線を引く
// ブレゼンハムのアルゴリズム
void	draw_2vec2(t_game *game, t_vec2 v1, t_vec2 v2, int color){
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
	printf("dx: %d, dy: %d\n", dx, dy);
	printf("sx: %d, sy: %d\n", sx, sy);
	if (dy <= dx) {
		int E = -dx;
		for (int i = 0; i <= dx; i++) {
			printf("dy <= dx\t x: %d, y: %d, E: %d\n", x, y, E);
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
			printf("dy > dx\t x: %d, y: %d, E: %d\n", x, y, E);
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

void	move_player(t_game *game)
{
	if (game->player.is_rotating)
	{
		game->player.angle = (game->player.angle += game->player.is_rotating * PLAYER_ROTATE_DEG) % 360;
	}
	if (game->player.is_moving)
	{
		double rad = deg2rad(game->player.angle);
		game->player.position.x += game->player.is_moving * PLAYER_MOVE_PX * cos(rad);
		game->player.position.y += game->player.is_moving * PLAYER_MOVE_PX * sin(rad);
	}
}

void	draw_wall(t_game *game)
{
	// 各壁ごとに縦横10pxのブロックを描画
	for(int i = 0; i < sizeof(MAP) / sizeof(MAP[0]); i++){
	  for (int j = 0; j < ft_strlen(MAP[i]); j++){
		if (MAP[i][j] == '1'){
			draw_block(game, j * BLOCK_SIZE, i * BLOCK_SIZE, 0x00FFFFFF);
		}
	  }
	}
}

void	draw_player(t_game *game)
{
	draw_line_angle_length(game, game->player.position, game->player.angle, BLOCK_SIZE / 2, 0x00FF0000);

	double rad = deg2rad(game->player.angle);
	// ブレゼンハムのアルゴリズムのテスト
	t_vec2 v1_ = {100, 0};
	t_vec2 v2_ = {0, 100};
	draw_2vec2(game, v1_, v2_, 0x000000FF);
	t_vec2 v3_ = {0, 0};
	t_vec2 v4_ = {100, 100};
	draw_2vec2(game, v3_, v4_, 0x0000FFFF);

	// Playerを三角形で描画
	int angle = -1 * game->player.angle;
	t_vec2 player = game->player.position;
	int length = 50;
	printf("angle  : %d\n", angle);
	t_vec2 v1 = player;
	t_vec2 v2 = {player.x + length * cos(deg2rad(angle + 150)), player.y + length * sin(deg2rad(angle + 150))};
	t_vec2 v3 = {player.x + length * cos(deg2rad(angle - 150)), player.y + length * sin(deg2rad(angle - 150))};
	printf("v2\tx: %d, y: %d\n", v2.x, v2.y);
	printf("v3\tx: %d, y: %d\n", v3.x, v3.y);
	draw_2vec2(game, v1, v2, 0x000000FF);
	// draw_2vec2(game, v2, v3, 0x0000FF00);
	draw_2vec2(game, v3, v1, 0x00FF00FF);
}

void	initialize_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, WIDTH, HEIGHT, "Hello world!");
    game->img.img = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    game->img.addr = mlx_get_data_addr(game->img.img, &game->img.bits_per_pixel, &game->img.line_length, &game->img.endian);
	game->map = MAP;

	game->player.position.x = 0;
	game->player.position.y = 0;
	game->player.is_moving = 0;
	game->player.angle = 0;
	game->player.is_rotating = 0;

	// Game Settings
	mlx_do_key_autorepeaton(game->mlx);
}

int		main_loop(t_game *game)
{
	move_player(game);
	clear_img(game);
	draw_wall(game);
	draw_player(game);
	print_game(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int main(int argc, char **argv){
	t_game	game;
	initialize_game(&game);

	mlx_hook(game.win, KeyPress, KeyPressMask, key_press_hook, &game);
	mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release_hook, &game);
	mlx_loop_hook(game.mlx, &main_loop, &game);
    mlx_loop(game.mlx);
}
