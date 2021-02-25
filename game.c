#include "cub3d.h"

void	configure_screen(t_game *game, bool has_window)
{
	if (has_window)
		game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "Hello world!");

	game->img.img = mlx_new_image(game->mlx, game->screen_width, game->screen_height);
	game->img.addr = mlx_get_data_addr(game->img.img, &game->img.bits_per_pixel, &game->img.line_length, &game->img.endian);
	game->img.width = game->screen_width;
	game->img.height = game->screen_height;

	game->tex_width = game->tex_n.width;
	game->tex_height = game->tex_n.height;

	// スプライト用
	game->z_buffer = ft_calloc(game->screen_width, sizeof(double));

	// Game Settings
	mlx_do_key_autorepeaton(game->mlx);
}

void	initialize_game(t_game *game)
{
	game->mlx = mlx_init();

	// MAP
	game->map = ft_calloc(MAX_MAP_HEIGHT, sizeof(char*));  // 200 * 200が最大MAPサイズ
	game->map_row = 0;
	game->map_col = 0;

	// player and sprite
	game->sprite_num = 0;
	game->sprites = NULL;
	game->player.pos.x = PLAYER_INIT_POS_X;
	game->player.pos.y = PLAYER_INIT_POS_Y;

	// textures
	game->tex_n.img = NULL;
	game->tex_s.img = NULL;
	game->tex_w.img = NULL;
	game->tex_e.img = NULL;
	game->tex_sprite.img = NULL;
	// cubfileで指定出来るのはRGBのみなのでUINT32_MAXが入ることはない
	game->ground_color = UINT32_MAX;
	game->sky_color = UINT32_MAX;

	game->screen_width = 0;
	game->screen_height = 0;
}

