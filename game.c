/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:13:06 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 23:35:24 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		configure_screen(t_game *game, bool has_window)
{
	double	plane_length;
	int		max_width;
	int		max_height;

	mlx_get_screen_size(game->mlx, &max_width, &max_height);
	printf("Display size\n\twidth: %d\n\theight: %d\n", max_width, max_height);
	if (has_window)
	{
		game->screen_width = MIN(game->screen_width, max_width);
		game->screen_height = MIN(game->screen_height, max_height);
		game->win = mlx_new_window(game->mlx,
			game->screen_width, game->screen_height, "cub3D");
	}
	game->img.img = mlx_new_image(game->mlx,
		game->screen_width, game->screen_height);
	game->img.addr = mlx_get_data_addr(game->img.img,
		&game->img.bits_per_pixel, &game->img.line_length, &game->img.endian);
	game->img.width = game->screen_width;
	game->img.height = game->screen_height;
	game->tex_width = game->tex_n.width;
	game->tex_height = game->tex_n.height;
	if (!(game->z_buffer = ft_calloc(game->screen_width, sizeof(double))) ||
		!(game->sprite_dists = ft_calloc(game->sprite_num, sizeof(double))))
		return (put_and_return_err("malloc failed"));
	plane_length = vec2_length(game->player.plane);
	game->height_base = (double)game->screen_width * (1 / (2 * plane_length));
	mlx_do_key_autorepeaton(game->mlx);
	return (0);
}

int		initialize_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!(game->map = ft_calloc(MAX_MAP_HEIGHT, sizeof(char*))))
		return (put_and_return_err("failed malloc"));
	game->map_row = 0;
	game->map_col = 0;
	game->sprite_num = 0;
	game->sprites = NULL;
	game->player.pos.x = PLAYER_INIT_POS_X;
	game->player.pos.y = PLAYER_INIT_POS_Y;
	game->tex_n.img = NULL;
	game->tex_s.img = NULL;
	game->tex_w.img = NULL;
	game->tex_e.img = NULL;
	game->tex_sprite.img = NULL;
	game->ground_color = UINT32_MAX;
	game->sky_color = UINT32_MAX;
	game->screen_width = 0;
	game->screen_height = 0;
	return (0);
}
