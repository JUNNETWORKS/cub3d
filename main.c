#include "cub3d.h"

int		main_loop(t_game *game)
{
	clear_img(&game->img);
	draw_walls(game);
	draw_sprites(game);
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
		if (configure_screen(&game, false))
			return (EXIT_FAILURE);
		draw_walls(&game);
		draw_sprites(&game);
		write_game2bmp(&game, "output.bmp");
		exit(EXIT_SUCCESS);
	}
	if (configure_screen(&game, true))
		return (EXIT_FAILURE);
	mlx_hook(game.win, KeyPress, KeyPressMask, key_press_hook, &game);
	mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release_hook, &game);
	mlx_loop_hook(game.mlx, &main_loop, &game);
	mlx_loop(game.mlx);
}
