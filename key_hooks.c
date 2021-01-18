#include "./cub3d.h"

// キーボード入力のフック
int 			key_hook(int keycode, t_game *game)
{
	printf("Pressed Key Code: %d\n", keycode);
	if (keycode == KEY_q || keycode == KEY_esc){
		mlx_destroy_window(game->mlx, game->win);
		exit(0);
	}
	if (keycode == KEY_w){
		double rad = (double)game->player.angle / 180 * M_PI;
		game->player.position.x += PLAYER_MOVE_PX * cos(rad);
		game->player.position.y += PLAYER_MOVE_PX * sin(rad);
	}
	if (keycode == KEY_s){
		double rad = (double)game->player.angle / 180 * M_PI;
		game->player.position.x -= PLAYER_MOVE_PX * cos(rad);
		game->player.position.y -= PLAYER_MOVE_PX * sin(rad);
	}
	if (keycode == KEY_a)
		game->player.angle = (game->player.angle -= 30) % 360;
	if (keycode == KEY_d)
		game->player.angle = (game->player.angle += 30) % 360;
	print_game(game);
	return (0);
}
