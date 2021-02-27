#include "cub3d.h"

void	draw_minimap(t_game *game)
{
	// マップの描画
	for (int i = 0; i < game->map_row; i++){
	  for (int j = 0; j < game->map_col; j++){
		// まずマスの端を描画して, 壁ならば中を塗りつぶす
		// for (int x = TILE_SIZE * j; x < TILE_SIZE * (j + 1); x++)
		if (game->map[i][j] == '1')
			draw_block(&game->img, BLOCK_SIZE * j, BLOCK_SIZE * i, 0x000000);
		else if (game->map[i][j] == '2')
			draw_block(&game->img, BLOCK_SIZE * j, BLOCK_SIZE * i, 0x00ff00);
		else
			draw_block(&game->img, BLOCK_SIZE * j, BLOCK_SIZE * i, 0xffffff);
	  }
	}
	// プレイヤーの描画
	for (int x = game->player.pos.x * BLOCK_SIZE - BLOCK_SIZE / 2; x < game->player.pos.x * BLOCK_SIZE + BLOCK_SIZE / 2; x++){
		for (int y = game->player.pos.y * BLOCK_SIZE - BLOCK_SIZE / 2; y < game->player.pos.y * BLOCK_SIZE + BLOCK_SIZE / 2; y++){
			if (x < 0 || x >= game->map_col * BLOCK_SIZE || y < 0 || y >= game->map_row * BLOCK_SIZE)
				continue;
			my_mlx_pixel_put(&game->img, x, y, 0xff0000);
		}
	}
	// visualize what direction player is seeing
	t_vec2 pos;
	pos.x = game->player.pos.x * BLOCK_SIZE;
	pos.y = game->player.pos.y * BLOCK_SIZE;
	t_vec2 dir_line;
	dir_line.x = pos.x + game->player.dir.x * BLOCK_SIZE;
	dir_line.y = pos.y + game->player.dir.y * BLOCK_SIZE;
	draw_2vec2(&game->img, pos, dir_line, 0x0000ff, 3);
	// visualize plane
	t_vec2 plane_left;
	plane_left.x = dir_line.x - game->player.plane.x * BLOCK_SIZE;
	plane_left.y = dir_line.y - game->player.plane.y * BLOCK_SIZE;
	t_vec2 plane_right;
	plane_right.x = dir_line.x + game->player.plane.x * BLOCK_SIZE;
	plane_right.y = dir_line.y + game->player.plane.y * BLOCK_SIZE;
	draw_2vec2(&game->img, pos, plane_left, 0x00ff00, 3);
	draw_2vec2(&game->img, pos, plane_right, 0x00ff00, 3);
	draw_2vec2(&game->img, plane_left, plane_right, 0x00ff00, 3);
	// lengthen left and right plane vector
	double lengthen_power = BLOCK_SIZE * 200;
	t_vec2 long_dir_line;
	long_dir_line.x = pos.x + game->player.dir.x * lengthen_power;
	long_dir_line.y = pos.y + game->player.dir.y * lengthen_power;
	t_vec2 long_plane_left;
	long_plane_left.x = long_dir_line.x - game->player.plane.x * lengthen_power;
	long_plane_left.y = long_dir_line.y - game->player.plane.y * lengthen_power;
	t_vec2 long_plane_right;
	long_plane_right.x = long_dir_line.x + game->player.plane.x * lengthen_power;
	long_plane_right.y = long_dir_line.y + game->player.plane.y * lengthen_power;
	draw_2vec2(&game->img, pos, long_plane_left, 0xffff00, 3);
	draw_2vec2(&game->img, pos, long_plane_right, 0xffff00, 3);
}


int		main_loop(t_game *game)
{
	clear_img(&game->img);
	draw_walls(game);
	draw_sprites(game);
	draw_minimap(game);
	update_player(game);
	print_game(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int main(int argc, char **argv)
{
	if (argc < 2 || argc > 3){  // ./cub3D *.cub --save
		put_error_msg("args count is incorrect!");
		exit(EXIT_FAILURE);
	}
	t_game	game;
	if ((initialize_game(&game)) == ERROR || (load_cubfile(&game, argv[1])) == ERROR){
		put_error_msg("Error is occured when load cub file");
		exit(EXIT_FAILURE);
	}
	if (argc == 3){
		if (ft_strncmp(argv[2], "--save", ft_strlen("--save") + 1)){
			put_error_msg("argv is not \"--save\"\n");
			exit(EXIT_FAILURE);
		}
		configure_screen(&game, false);
		draw_walls(&game);
		draw_sprites(&game);
		write_game2bmp(&game, "output.bmp");
		exit(EXIT_SUCCESS);
	}
	configure_screen(&game, true);
	mlx_hook(game.win, KeyPress, KeyPressMask, key_press_hook, &game);
	mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release_hook, &game);
	mlx_loop_hook(game.mlx, &main_loop, &game);
	mlx_loop(game.mlx);
}
