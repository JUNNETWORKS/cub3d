/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:00:26 by jtanaka           #+#    #+#             */
/*   Updated: 2021/03/02 03:42:33 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	initialize_ray(t_game *game, t_ray *ray, int x)
{
	ray->camera_x = 2 * x / (double)game->screen_width - 1;
	ray->dir.x = game->player.dir.x + game->player.plane.x * ray->camera_x;
	ray->dir.y = game->player.dir.y + game->player.plane.y * ray->camera_x;
	ray->map_x = (int)game->player.pos.x;
	ray->map_y = (int)game->player.pos.y;
	ray->delta_dist_x = (1 / ray->dir.x) < 0 ?
		-1 * (1 / ray->dir.x) : (1 / ray->dir.x);
	ray->delta_dist_y = (1 / ray->dir.y) < 0 ?
		-1 * (1 / ray->dir.y) : (1 / ray->dir.y);
	ray->step_x = ray->dir.x < 0 ? -1 : 1;
	ray->side_dist_x = ray->dir.x < 0 ?
		(game->player.pos.x - ray->map_x) * ray->delta_dist_x :
		(ray->map_x + 1.0 - game->player.pos.x) * ray->delta_dist_x;
	ray->step_y = ray->dir.y < 0 ? -1 : 1;
	ray->side_dist_y = ray->dir.y < 0 ?
		(game->player.pos.y - ray->map_y) * ray->delta_dist_y :
		(ray->map_y + 1.0 - game->player.pos.y) * ray->delta_dist_y;
}

static void	simulate_ray(t_game *game, t_ray *ray)
{
	while (game->map[ray->map_y][ray->map_x] != '1')
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
	}
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player.pos.x +
			(1 - ray->step_x) / 2) / ray->dir.x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player.pos.y +
			(1 - ray->step_y) / 2) / ray->dir.y;
	if (ray->side == 0)
		ray->tex = (ray->step_x > 0) ? &game->tex_w : &game->tex_e;
	else
		ray->tex = (ray->step_y > 0) ? &game->tex_n : &game->tex_s;
}

static void	calc_vis_info(t_game *game, t_ray ray,
						t_wall_vis_info *wall_vis, int x)
{
	wall_vis->line_height = (int)(game->height_base / ray.perp_wall_dist);
	wall_vis->draw_start = -wall_vis->line_height / 2 + game->screen_height / 2;
	if (wall_vis->draw_start < 0)
		wall_vis->draw_start = 0;
	wall_vis->draw_end = wall_vis->line_height / 2 + game->screen_height / 2;
	if (wall_vis->draw_end >= game->screen_height)
		wall_vis->draw_end = game->screen_height - 1;
	if (ray.side == 0)
		wall_vis->wall_x = game->player.pos.y + ray.perp_wall_dist * ray.dir.y;
	else
		wall_vis->wall_x = game->player.pos.x + ray.perp_wall_dist * ray.dir.x;
	wall_vis->wall_x -= floor(wall_vis->wall_x);
	wall_vis->texture_x = (int)(wall_vis->wall_x * ray.tex->width);
	if ((ray.side == 0 && ray.dir.x < 0) || (ray.side == 1 && ray.dir.y > 0))
		wall_vis->texture_x = ray.tex->width - wall_vis->texture_x - 1;
	wall_vis->step = 1.0 * ray.tex->height / (double)wall_vis->line_height;
}

static void	draw_stripe(t_game *game, t_ray ray,
						t_wall_vis_info *wall_vis, int x)
{
	int				y;
	uint32_t		color;

	wall_vis->texture_pos_y =
		(wall_vis->draw_start - game->screen_height / 2
		+ wall_vis->line_height / 2) * wall_vis->step;
	y = 0;
	while (y < game->screen_height)
	{
		if (y <= game->screen_height / 2)
			my_mlx_pixel_put(&(game->img), x, y, game->sky_color);
		else
			my_mlx_pixel_put(&(game->img), x, y, game->ground_color);
		if (y >= wall_vis->draw_start && y < wall_vis->draw_end)
		{
			wall_vis->texture_y = (int)wall_vis->texture_pos_y;
			if (wall_vis->texture_y >= ray.tex->height)
				wall_vis->texture_y = ray.tex->height - 1;
			wall_vis->texture_pos_y += wall_vis->step;
			color = get_color(*ray.tex,
				wall_vis->texture_x, wall_vis->texture_y);
			my_mlx_pixel_put(&(game->img), x, y, color);
		}
		y++;
	}
}

void		draw_walls(t_game *game)
{
	int				x;
	t_ray			ray;
	t_wall_vis_info	wall_vis;

	x = 0;
	while (x < game->screen_width)
	{
		initialize_ray(game, &ray, x);
		simulate_ray(game, &ray);
		game->z_buffer[x] = ray.perp_wall_dist;
		calc_vis_info(game, ray, &wall_vis, x);
		draw_stripe(game, ray, &wall_vis, x);
		x++;
	}
}
