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

void	move_player(t_game *game)
{
	if (game->player.is_rotating)
	{
		game->player.angle = game->player.angle + game->player.is_rotating * PLAYER_ROTATE_RAD;
		/*
		if (game->player.angle >= 2 * M_PI)
			game->player.angle -= 2 * M_PI;
		else if (game->player.angle <= -2 * M_PI)
			game->player.angle += 2 * M_PI;
			*/
	}
	if (game->player.is_moving)
	{
		game->player.position.x += game->player.is_moving * PLAYER_MOVE_PX * cos(game->player.angle);
		game->player.position.y += game->player.is_moving * PLAYER_MOVE_PX * sin(game->player.angle);
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
	draw_line_angle_length(game, game->player.position, game->player.angle, 20, 0x00FF0000);

	// Playerを三角形で描画
	int angle = -1 * game->player.angle;
	t_vec2 player = game->player.position;
	int length = 20;
	t_vec2 v1 = player;
	t_vec2 v2 = {player.x + length * cos(game->player.angle + 5 / 6 * M_PI), player.y - length * sin(game->player.angle + 5 / 6 * M_PI)};
	t_vec2 v3 = {player.x + length * cos(game->player.angle - 5 / 6 * M_PI), player.y - length * sin(game->player.angle - 5 / 6 * M_PI)};
	draw_2vec2(game, v1, v2, 0x000000FF);
	draw_2vec2(game, v2, v3, 0x0000FF00);
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
