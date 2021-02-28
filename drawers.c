/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:07:07 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 18:07:08 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

void	clear_img(t_img *img)
{
	int	x;
	int	y;

	x = 0;
	while (x < img->width)
	{
		y = 0;
		while (y < img->height)
		{
			my_mlx_pixel_put(img, x, y, 0xff000000);
			y++;
		}
		x++;
	}
}

void	draw_block(t_img *img, int x, int y, int color)
{
	int	xx;
	int	yy;

	xx = 0;
	while (xx < BLOCK_SIZE)
	{
		yy = 0;
		while (yy < BLOCK_SIZE)
		{
			my_mlx_pixel_put(img, x + xx, y + yy, color);
			yy++;
		}
		xx++;
	}
}
