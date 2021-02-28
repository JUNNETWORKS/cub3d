/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floodfill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:11:48 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 18:11:49 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static bool	floodfill(t_game *game, bool **filled_map, int i, int j)
{
	bool	is_surrounded;

	if (i < 0 || i >= game->map_row || j < 0 || j >= game->map_col)
		return (false);
	if (game->map[i][j] == '1' || filled_map[i][j] == true)
		return (true);
	else
		filled_map[i][j] = true;
	is_surrounded = true;
	is_surrounded &= floodfill(game, filled_map, i - 1, j);
	is_surrounded &= floodfill(game, filled_map, i + 1, j);
	is_surrounded &= floodfill(game, filled_map, i, j - 1);
	is_surrounded &= floodfill(game, filled_map, i, j + 1);
	return (is_surrounded);
}

int			check_map_surrounded(t_game *game)
{
	int		x;
	int		y;
	int		i;
	bool	**filled_map;
	bool	is_surrounded;

	x = game->player.pos.x;
	y = game->player.pos.y;
	filled_map = ft_calloc(game->map_row + 1, sizeof(bool*));
	i = 0;
	while (i < game->map_row)
	{
		filled_map[i] = ft_calloc(game->map_col, sizeof(bool));
		if (!filled_map[i])
		{
			free_ptrarr((void**)filled_map);
			return (put_and_return_err("Malloc is failed"));
		}
		i++;
	}
	is_surrounded = floodfill(game, filled_map, y, x);
	free_ptrarr((void**)filled_map);
	if (!is_surrounded)
		return (put_and_return_err("Map isn't surrounded by wall"));
	return (0);
}
