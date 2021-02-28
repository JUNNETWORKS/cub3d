/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 23:51:10 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 23:51:12 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void		my_mlx_pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	dst = img->addr + (y * img->line_length +
		x * (img->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

uint32_t	get_color(t_img img, int x, int y)
{
	return (*(uint32_t*)(img.addr +
		(y * img.line_length + x * (img.bits_per_pixel / 8))));
}

int			load_image(t_game *game, t_img *img, char *filepath)
{
	img->img = mlx_xpm_file_to_image(game->mlx, filepath,
		&img->width, &img->height);
	if (img->img == NULL)
		return (ERROR);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
		&img->line_length, &img->endian);
	if (img->addr == NULL)
		return (ERROR);
	return (0);
}
