/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:57:16 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 16:59:11 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calc_transform(t_game *game, t_vec2 sprite,
			double *transform_x, double *transform_y)
{
	double inv_det;
	t_vec2 normalized_sprite;

	normalized_sprite.x = sprite.x - game->player.pos.x;
	normalized_sprite.y = sprite.y - game->player.pos.y;
	inv_det = 1.0 / (game->player.plane.x *
		game->player.dir.y - game->player.dir.x * game->player.plane.y);
	*transform_x = inv_det * (game->player.dir.y *
		normalized_sprite.x - game->player.dir.x * normalized_sprite.y);
	*transform_y = inv_det * (-game->player.plane.y *
		normalized_sprite.x + game->player.plane.x * normalized_sprite.y);
}

void	draw_sprite_stripe(t_game *game, t_sprite_vis_info sprite_vis)
{
	int			x;
	int			y;
	int			tex_x;
	int			tex_y;
	uint32_t	color;

	x = sprite_vis.draw_start_x;
	while(x < sprite_vis.draw_end_x)
	{
		tex_x = (int)((x - (-sprite_vis.width_screen / 2 + sprite_vis.screen_x)) *
			game->tex_sprite.width / sprite_vis.width_screen);
		if (sprite_vis.transform_y > 0 &&
			x >= 0 && x < game->screen_width &&
			sprite_vis.transform_y < game->z_buffer[x]){
			y = sprite_vis.draw_start_y;
			while(y < sprite_vis.draw_end_y)
			{
				tex_y = (int)(
					(y - (-sprite_vis.height_screen / 2 + game->screen_height / 2)) *
					game->tex_sprite.height / sprite_vis.height_screen);
				color = get_color_from_img(game->tex_sprite, tex_x, tex_y);
				if (!(color & 0xff000000))
					my_mlx_pixel_put(&(game->img), x, y, color);
				y++;
			}
		}
		x++;
	}
}

void	draw_sprite(t_game *game, t_vec2 sprite)
{
	t_sprite_vis_info	sprite_vis;

	calc_transform(game, sprite,
		&sprite_vis.transform_x, &sprite_vis.transform_y);
	sprite_vis.screen_x = (int)((game->screen_width / 2) *
		(1.0 + sprite_vis.transform_x / sprite_vis.transform_y));
	sprite_vis.height_screen =
		ABS((int)(game->height_base / sprite_vis.transform_y));
	sprite_vis.draw_start_y =
		-sprite_vis.height_screen / 2 + game->screen_height / 2;
	if (sprite_vis.draw_start_y < 0)
		sprite_vis.draw_start_y = 0;
	sprite_vis.draw_end_y = sprite_vis.height_screen / 2 +
		game->screen_height / 2;
	if (sprite_vis.draw_end_y >= game->screen_height)
		sprite_vis.draw_end_y = (sprite_vis.draw_end_y >= game->screen_height) ?
			game->screen_height - 1 : sprite_vis.draw_end_y;
	sprite_vis.width_screen =
		ABS((int)(game->height_base / sprite_vis.transform_y));
	sprite_vis.draw_start_x =
		-sprite_vis.width_screen / 2 + sprite_vis.screen_x;
	if (sprite_vis.draw_start_x < 0)
		sprite_vis.draw_start_x = 0;
	sprite_vis.draw_end_x = sprite_vis.width_screen / 2 +
		sprite_vis.screen_x;
	if (sprite_vis.draw_end_x >= game->screen_width)
		sprite_vis.draw_end_x = game->screen_width - 1;
	draw_sprite_stripe(game, sprite_vis);
}

void	draw_sprites(t_game *game)
{
	double	plane_length;
	int		sprite_idx;

	sort_sprites(game);
	sprite_idx = 0;
	while (sprite_idx < game->sprite_num)
		draw_sprite(game, game->sprites[sprite_idx++]);
}
