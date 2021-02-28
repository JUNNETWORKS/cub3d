/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_cub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:18:14 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 18:41:24 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

static int	get_pos_from_map(t_game *game)
{
	int i;
	int j;
	
	i = 0;
	while (i < game->map_row)
	{
		j = 0;
		while (j < game->map_col)
		{
			if (game->map[i][j] == '\0' && ++j)
				continue;
			if (!ft_strchr(" 012NSWE", game->map[i][j]))
				return (put_and_return_err("The map has invalid character"));
			if (game->map[i][j] == '2' && add_sprite(game, j + 0.5, i + 0.5) == ERROR)
				return (put_and_return_err("malloc is failed"));
			else if (ft_strchr("NSWE", game->map[i][j]))
			{
				if (game->player.pos.x != PLAYER_INIT_POS_X &&
					game->player.pos.y != PLAYER_INIT_POS_Y)
					return (put_and_return_err("Player's position must be unique"));
				initialize_player(&game->player, j + 0.5, i + 0.5, game->map[i][j]);
			}
			j++;
		}
		i++;
	}
	return (0);
}

static bool	has_config_already_set(t_game *game)
{
	if (!game->tex_n.img || !game->tex_s.img || !game->tex_w.img ||
		!game->tex_e.img || !game->tex_sprite.img ||
		game->ground_color == UINT32_MAX || game->sky_color == UINT32_MAX ||
		game->screen_width == -1 || game->screen_height == -1)
		return (false);
	return (true);
}

int			load_map(t_game *game, char *line)
{
	if (!has_config_already_set(game))
		return (put_and_return_err("Must configure all elements before loading map"));
	if (!line || ft_strlen(line) >= MAX_MAP_WIDTH || game->map_row >= MAX_MAP_WIDTH)
		return (put_and_return_err("map is too large"));
	game->map[game->map_row] = ft_calloc(MAX_MAP_WIDTH, sizeof(char));
	ft_strlcpy(game->map[game->map_row], line, ft_strlen(line) + 1);
	if (!game->map[game->map_row])
		return (put_and_return_err("error strdup()"));
	game->map_row++;
	game->map_col = ft_strlen(line) > game->map_col ? ft_strlen(line) : game->map_col;
	return (0);
}

bool		is_valid_cubpath(char *path)
{
	size_t	path_len;

	path_len = ft_strlen(path);
	return !(path_len < 5 || path[path_len - 5] == '/'
			|| ft_strncmp(path + path_len - 4, ".cub", 4));
}

bool		is_texture_identifier(char *idenfier)
{
	return (idenfier[0] == 'S' || !ft_strncmp(idenfier, "NO", 3)
			|| !ft_strncmp(idenfier, "SO", 3) || !ft_strncmp(idenfier, "WE", 3)
			|| !ft_strncmp(idenfier, "EA", 3));
}

int			load_cubfile_fd(t_game *game, int fd)
{
	char	*line;
	int		status;
	char	**params;

	status = 0;
	while (status >= 0 && (status = get_next_line(fd, &line)) == 1)
	{
		status = !(params = ft_split(line, ' ')) ? ERROR : status;
		if (status >= 0 && params[0] &&
			ft_strnstr(params[0], "R", ft_strlen(params[0])))
			status = set_resolution(game, params[1], params[2]);
		else if ((status >= 0 && params[0]) &&
			(params[0][0] == 'F' || params[0][0] == 'C'))
			status = set_color(game, params[0][0], params[1]);
		else if (status >= 0 && params[0] && is_texture_identifier(params[0]))
			status = load_texture(game, params[0], params[1]);
		else if (status >= 0 && params[0])
			status = load_map(game, line);
		free_and_assign_null((void**)&line);
		free_ptrarr((void**)params);
	}
	free(line);
	return (status);
}

int			load_cubfile(t_game *game, char *path)
{
	int		fd;

	if (!is_valid_cubpath(path))
		return (put_and_return_err("File extension is not .cub"));
	if ((fd = open(path, O_RDONLY)) == -1)
		return (put_and_return_err("Failed to open file"));
	if (load_cubfile_fd(game, fd) == ERROR)
		return (put_and_return_err("Error occured during load cubfile"));
	if (get_pos_from_map(game) || check_map_surrounded(game))
		return (put_and_return_err("Error occured during load map"));
	if (game->player.pos.x == PLAYER_INIT_POS_X && game->player.pos.y == PLAYER_INIT_POS_Y)
		return (put_and_return_err("Player's position is not specified"));

	return (0);
}
