#include "./cub3d.h"

// キーボード入力のフック
int 			key_hook(int keycode, t_game *game)
{
	printf("Pressed Key Code: %d\n", keycode);
	if (keycode == KEY_q || keycode == KEY_esc){
		mlx_destroy_window(game->mlx, game->win);
		exit(0);
	}
	return (0);
}
