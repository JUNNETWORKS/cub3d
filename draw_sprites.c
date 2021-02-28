/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:57:16 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 17:54:02 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	calc_transform(t_game *game, t_vec2 sprite,
			double *transform_x, double *transform_y)
{
	double	inv_det;
	t_vec2	normalized_sprite;

	normalized_sprite.x = sprite.x - game->player.pos.x;
	normalized_sprite.y = sprite.y - game->player.pos.y;
	inv_det = 1.0 / (game->player.plane.x *
		game->player.dir.y - game->player.dir.x * game->player.plane.y);
	*transform_x = inv_det * (game->player.dir.y *
		normalized_sprite.x - game->player.dir.x * normalized_sprite.y);
	*transform_y = inv_det * (-game->player.plane.y *
		normalized_sprite.x + game->player.plane.x * normalized_sprite.y);
}

static void	calc_sprite_vis(t_game *game,
	t_sprite_vis_info *sprite_vis, t_vec2 sprite)
{
	calc_transform(game, sprite,
		&sprite_vis->transform_x, &sprite_vis->transform_y);
	sprite_vis->screen_x = (int)((game->screen_width / 2) *
		(1.0 + sprite_vis->transform_x / sprite_vis->transform_y));
	sprite_vis->height =
		ABS((int)(game->height_base / sprite_vis->transform_y));
	sprite_vis->draw_start_y =
		-sprite_vis->height / 2 + game->screen_height / 2;
	if (sprite_vis->draw_start_y < 0)
		sprite_vis->draw_start_y = 0;
	sprite_vis->draw_end_y = sprite_vis->height / 2 +
		game->screen_height / 2;
	sprite_vis->draw_end_y =
		(sprite_vis->draw_end_y >= game->screen_height) ?
		game->screen_height - 1 : sprite_vis->draw_end_y;
	sprite_vis->width =
		ABS((int)(game->height_base / sprite_vis->transform_y));
	sprite_vis->draw_start_x =
		-sprite_vis->width / 2 + sprite_vis->screen_x;
	if (sprite_vis->draw_start_x < 0)
		sprite_vis->draw_start_x = 0;
	sprite_vis->draw_end_x = sprite_vis->width / 2 +
		sprite_vis->screen_x;
	if (sprite_vis->draw_end_x >= game->screen_width)
		sprite_vis->draw_end_x = game->screen_width - 1;
}

void		draw_sprites(t_game *game)
{
	double				plane_length;
	int					sprite_idx;
	t_sprite_vis_info	sprite_vis;

	sort_sprites(game);
	sprite_idx = 0;
	while (sprite_idx < game->sprite_num)
	{
		calc_sprite_vis(game, &sprite_vis, game->sprites[sprite_idx++]);
		draw_sprite_stripe(game, sprite_vis);
	}
}
