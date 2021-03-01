/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_cub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:18:14 by jtanaka           #+#    #+#             */
/*   Updated: 2021/03/02 04:34:40 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

bool		is_valid_cubpath(char *path)
{
	size_t	path_len;

	path_len = ft_strlen(path);
	return (!(path_len < 5 || path[path_len - 5] == '/' ||
		ft_strncmp(path + path_len - 4, ".cub", 4)));
}

bool		is_texture_identifier(char *idenfier)
{
	return (idenfier[0] == 'S'
			|| !ft_strncmp(idenfier, "NO", 3)
			|| !ft_strncmp(idenfier, "SO", 3)
			|| !ft_strncmp(idenfier, "WE", 3)
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
		params = ft_split(line, ' ');
		status = !(params) ? ERROR : status;
		status = game->map_row && !params[0] ? ERROR : status;
		if (status >= 0 && params[0] && get_split_size(line, ' ') == 3 &&
			ft_strnstr(params[0], "R", ft_strlen(params[0])))
			status = set_resolution(game, params[1], params[2]);
		else if ((status >= 0 && params[0]) && is_c_f_or_c(params[0][0]))
			status = set_color(game, params[0][0], params[1]);
		else if (status >= 0 && params[0] &&
			is_texture_identifier(params[0]))
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
	if (game->player.pos.x == PLAYER_INIT_POS_X &&
		game->player.pos.y == PLAYER_INIT_POS_Y)
		return (put_and_return_err("Player's position is not specified"));
	return (0);
}
