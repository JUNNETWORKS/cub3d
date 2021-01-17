#include "./cub3d.h"

// キーボード入力のフック
int 			key_hook(int keycode, t_game *vars)
{
	printf("Pressed Key Code: %d\n", keycode);
	if (keycode == KEY_q || keycode == KEY_esc){
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
	return (0);
}
