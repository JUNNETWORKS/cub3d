/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:51:53 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 16:52:07 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	write_file_header(t_game *game, int fd)
{
	uint32_t	tmp;

	write(fd, "BM", 2);
	tmp = 14 + 10 + game->screen_width * game->screen_height * 4;
	write(fd, &tmp, 4);
	write(fd, "\0\0", 2);
	write(fd, "\0\0", 2);
	tmp = 14 + 10;
	write(fd, &tmp, 4);
}

static void	write_information_header(t_game *game, int fd)
{
	uint32_t	tmp;

	tmp = 12;
	write(fd, &tmp, 4);
	write(fd, &game->screen_width, 2);
	write(fd, &game->screen_height, 2);
	tmp = 1;
	write(fd, &tmp, 2);
	tmp = 32;
	write(fd, &tmp, 2);
}

static void	write_image_data(t_game *game, int fd)
{
	uint32_t	tmp;
	int			x;
	int			y;

	y = game->screen_height - 1;
	while (y >= 0)
	{
		x = 0;
		while (x < game->screen_width)
		{
			tmp = get_color_from_img(game->img, x, y);
			write(fd, &tmp, 4);
			x++;
		}
		y--;
	}
}

int			write_game2bmp(t_game *game, char *fpath)
{
	int			fd;

	if ((fd = open(fpath, O_WRONLY | O_CREAT,
			S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
		return (-1);
	write_file_header(game, fd);
	write_information_header(game, fd);
	write_image_data(game, fd);
	close(fd);
	return (0);
}
