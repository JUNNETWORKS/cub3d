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
	double deltax = v2.x - v1.x;
	double deltay = v2.y - v1.y;
	double error = 0;
	double deltaerr = deltay / deltax < 0 ? -1 * (deltay / deltax) : (deltay / deltax);    // deltax != 0 と仮定（垂直な線は扱わない）
	// この除算は分数を保持する形で行う必要がある。
	int y = v1.y;
	int xmin = v1.x < v2.x ? v1.x : v2.x;
	int xmax = v1.x > v2.x ? v1.x : v2.x;
	for (int x = xmin; x <= xmax; x++){
		my_mlx_pixel_put(game, x, y, color);
		error = error + deltaerr;
		if (error >= 0.5){
			y++;
			error -= 1.0;
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
		double rad = (double)game->player.angle / 180 * M_PI;
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
