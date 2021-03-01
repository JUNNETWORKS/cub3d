/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_color.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:16:03 by jtanaka           #+#    #+#             */
/*   Updated: 2021/03/02 04:34:53 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool		is_c_f_or_c(char c)
{
	return (c == 'F' || c == 'C');
}

bool		is_valid_color(int r, int g, int b)
{
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (false);
	return (true);
}

uint32_t	get_color_from_rgbstr(char *rgbstr, uint32_t *color)
{
	char	**rgb;

	if (str_c_count(rgbstr, ',') != 2 || get_split_size(rgbstr, ',') != 3)
		return (put_and_return_err("rgb is wrong"));
	if (!(rgb = ft_split(rgbstr, ',')))
		return (ERROR);
	if (!str_all_true(rgb[0], ft_isdigit) ||
			!str_all_true(rgb[1], ft_isdigit) ||
			!str_all_true(rgb[2], ft_isdigit) ||
			!is_valid_color(ft_atoi(rgb[0]),
				ft_atoi(rgb[1]), ft_atoi(rgb[2])) ||
			(rgb[0][0] == '0' && rgb[0][1]) ||
			(rgb[1][0] == '0' && rgb[1][1]) ||
			(rgb[2][0] == '0' && rgb[2][1]))
	{
		free_ptrarr((void**)rgb);
		return (ERROR);
	}
	*color = ft_atoi(rgb[0]) << 16 | ft_atoi(rgb[1]) << 8 | ft_atoi(rgb[2]);
	free_ptrarr((void**)rgb);
	return (0);
}

bool		has_color_set(t_game *game, char name)
{
	if (name == 'F' && game->ground_color == UINT32_MAX)
		return (false);
	else if (name == 'C' && game->sky_color == UINT32_MAX)
		return (false);
	return (true);
}

int			set_color(t_game *game, char name, char *rgbstr)
{
	char		**rgb;
	uint32_t	color;
	bool		is_invalid;

	if (get_color_from_rgbstr(rgbstr, &color) == ERROR)
		return (put_and_return_err("provided color is invalid"));
	if (has_color_set(game, name))
		return (put_and_return_err("color has already set"));
	if (name == 'F')
		game->ground_color = color;
	else if (name == 'C')
		game->sky_color = color;
	else
		return (put_and_return_err("Unknow key is provided"));
	return (0);
}
