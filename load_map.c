/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 23:07:43 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 23:07:43 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int		get_pos_from_map(t_game *game)
{
	int i;
	int j;

	i = 0;
	while (i < game->map_row)
	{
		j = 0;
		while (j < game->map_col)
		{
			if (game->map[i][j] && !ft_strchr(" 012NSWE", game->map[i][j]))
				return (put_and_return_err("The map has invalid character"));
			if (game->map[i][j] && game->map[i][j] == '2' &&
				add_sprite(game, j + 0.5, i + 0.5) == ERROR)
				return (put_and_return_err("malloc is failed"));
			else if (game->map[i][j] && ft_strchr("NSWE", game->map[i][j]) &&
				initialize_player(&game->player,
				j + 0.5, i + 0.5, game->map[i][j]) == ERROR)
				return (put_and_return_err("Player's position must be unique"));
			j++;
		}
		i++;
	}
	return (0);
}

bool	has_config_already_set(t_game *game)
{
	if (!game->tex_n.img || !game->tex_s.img || !game->tex_w.img ||
		!game->tex_e.img || !game->tex_sprite.img ||
		game->ground_color == UINT32_MAX || game->sky_color == UINT32_MAX ||
		game->screen_width == -1 || game->screen_height == -1)
		return (false);
	return (true);
}

int		load_map(t_game *game, char *line)
{
	if (!has_config_already_set(game))
		return (put_and_return_err("configuration hasn't set"));
	if (!line ||
		ft_strlen(line) >= MAX_MAP_WIDTH || game->map_row >= MAX_MAP_WIDTH)
		return (put_and_return_err("map is too large"));
	game->map[game->map_row] = ft_calloc(MAX_MAP_WIDTH, sizeof(char));
	ft_strlcpy(game->map[game->map_row], line, ft_strlen(line) + 1);
	if (!game->map[game->map_row])
		return (put_and_return_err("error strdup()"));
	game->map_row++;
	game->map_col = ft_strlen(line) > game->map_col ?
		ft_strlen(line) : game->map_col;
	return (0);
}
