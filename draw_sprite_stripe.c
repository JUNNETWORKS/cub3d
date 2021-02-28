/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprite_stripe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 17:47:06 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 17:55:37 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	is_stripe_visible(t_game *game, t_sprite_vis_info sprite_vis, int x)
{
	return (sprite_vis.transform_y > 0 &&
			x >= 0 && x < game->screen_width &&
			sprite_vis.transform_y < game->z_buffer[x]);
}

static int	calc_tex_x(t_game *game, t_sprite_vis_info sprite_vis, int x)
{
	return ((int)((x - (-sprite_vis.width / 2 + sprite_vis.screen_x)) *
			game->tex_sprite.width / sprite_vis.width));
}

static int	calc_tex_y(t_game *game, t_sprite_vis_info sprite_vis, int y)
{
	return ((int)((y - (-sprite_vis.height / 2 + game->screen_height / 2)) *
					game->tex_sprite.height / sprite_vis.height));
}

void		draw_sprite_stripe(t_game *game, t_sprite_vis_info sprite_vis)
{
	int			x;
	int			y;
	int			tex_x;
	int			tex_y;

	x = sprite_vis.draw_start_x;
	while (x < sprite_vis.draw_end_x)
	{
		tex_x = calc_tex_x(game, sprite_vis, x);
		if (is_stripe_visible(game, sprite_vis, x))
		{
			y = sprite_vis.draw_start_y;
			while (y < sprite_vis.draw_end_y)
			{
				tex_y = calc_tex_y(game, sprite_vis, y);
				if (!(get_color(game->tex_sprite, tex_x, tex_y) & 0xff000000))
					my_mlx_pixel_put(&(game->img), x, y,
						get_color(game->tex_sprite, tex_x, tex_y));
				y++;
			}
		}
		x++;
	}
}
