/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 00:21:07 by jtanaka           #+#    #+#             */
/*   Updated: 2021/03/01 00:44:43 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		main_loop(t_game *game)
{
	clear_img(&game->img);
	draw_walls(game);
	draw_sprites(game);
	update_player(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int		main(int argc, char **argv)
{
	t_game	game;

	if (argc < 2 || argc > 3)
		put_and_exit_err("args count is incorrect!");
	if ((initialize_game(&game)) == ERROR ||
		(load_cubfile(&game, argv[1])) == ERROR)
		put_and_exit_err("Error is occured when load cub file");
	if (argc == 3)
	{
		if (ft_strncmp(argv[2], "--save", ft_strlen("--save") + 1))
			put_and_exit_err("argv is not \"--save\"");
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
