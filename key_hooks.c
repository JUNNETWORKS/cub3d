#include "./cub3d.h"

int 			key_press_hook(int keycode, t_game *game)
{
	// printf("Pressed Key Code: %d\n", keycode);
	if (keycode == KEY_q || keycode == KEY_esc){
		mlx_destroy_window(game->mlx, game->win);
		exit(0);
	}
	if (keycode == KEY_w)
		game->player.is_moving = 1;
	if (keycode == KEY_s)
		game->player.is_moving = -1;
	if (keycode == KEY_a)
		game->player.is_sidling = 1;
	if (keycode == KEY_d)
		game->player.is_sidling = -1;
	if (keycode == KEY_lallow)
		game->player.is_rotating = 1;
	if (keycode == KEY_rallow)
		game->player.is_rotating = -1;
	return (0);
}

int 			key_release_hook(int keycode, t_game *game)
{
	if (keycode == KEY_w || keycode == KEY_s)
		game->player.is_moving = 0;
	if (keycode == KEY_a || keycode == KEY_d)
		game->player.is_sidling = 0;
	if (keycode == KEY_rallow || keycode == KEY_lallow)
		game->player.is_rotating = 0;
	return (0);
}
