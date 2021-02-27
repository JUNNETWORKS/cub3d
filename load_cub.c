#include "./cub3d.h"

// get player and sprite positions from map
int		get_pos_from_map(t_game *game)
{
	for (int i = 0; i < game->map_row; i++){
		for (int j = 0; j < game->map_col; j++){
			if (game->map[i][j] == '\0')
				continue;
			if (!ft_strchr(" 012NSWE", game->map[i][j]))
				return (put_and_return_err("The map has invalid character"));
			if (game->map[i][j] == '2'){
				printf("add sprite {x: %lf, y: %lf}\n", j + 0.5, i + 0.5);
				if (add_sprite(game, j + 0.5, i + 0.5) == ERROR)
					return (put_and_return_err("malloc is failed"));
			}
			else if (ft_strchr("NSWE", game->map[i][j])){
				if (game->player.pos.x != PLAYER_INIT_POS_X &&
					game->player.pos.y != PLAYER_INIT_POS_Y)
					return (put_and_return_err("Player's position must be unique"));
				initialize_player(&game->player, j + 0.5, i + 0.5, game->map[i][j]);
			}
		}
	}
	// print sprites
	printf("-----------------------SPRITES--------------------\n");
	for (int i = 0; i < game->sprite_num; i++)
	  printf("sprites[%d] = {x: %lf, y: %lf}\n", i, game->sprites[i].x, game->sprites[i].y);
	// プレイヤーの初期座標が無い場合はエラー
	if (game->player.pos.x == PLAYER_INIT_POS_X && game->player.pos.y == PLAYER_INIT_POS_Y)
		return (put_and_return_err("Player's position is not specified"));
	return (0);
}


// マップ以外の設定項目が全て設定されているか
bool	is_config_already_set(t_game *game)
{
	if (!game->tex_n.img || !game->tex_s.img || !game->tex_w.img ||
		!game->tex_e.img || !game->tex_sprite.img ||
		game->ground_color == UINT32_MAX || game->sky_color == UINT32_MAX ||
		game->screen_width == -1 || game->screen_height == -1)
		return (false);
	return (true);
}

int	load_map(t_game *game, char *line)
{
	if (!is_config_already_set(game))
		return (put_and_return_err("Must configure all elements before loading map"));
	if (!line || ft_strlen(line) >= MAX_MAP_WIDTH || game->map_row >= MAX_MAP_WIDTH){
		put_error_msg("map is too large");
		return (ERROR);
	}
	game->map[game->map_row] = ft_calloc(MAX_MAP_WIDTH, sizeof(char));
	ft_strlcpy(game->map[game->map_row], line, ft_strlen(line) + 1);
	if (!game->map[game->map_row]){
		put_error_msg("error strdup()");
		return (ERROR);
	}
	game->map_row++;
	game->map_col = ft_strlen(line) > game->map_col ? ft_strlen(line) : game->map_col;
	return (0);
}

int	load_cubfile(t_game *game, char *path)
{
	int		fd;
	char	*line;
	int		status;
	char	**params;
	size_t	path_len;

	// cubfileの名前が正しいかチェックする(*.cubか)
	path_len = ft_strlen(path);
	if (path_len < 5 || path[path_len - 5] == '/'
		|| ft_strncmp(path + path_len - 4, ".cub", 4))
		return (put_and_return_err("File extension is not .cub"));
	if ((fd = open(path, O_RDONLY)) == -1)
		return (put_and_return_err("Failed to open file"));
	status = 0;
	while (status >= 0 && (status = get_next_line(fd, &line)) == 1)
	{
		if (!(params = ft_split(line, ' ')))
		{
			status = ERROR;
			break;
		}

		printf("params[0]: |%s|\n", params[0]);
		if (params[0] == NULL)
		{
			free(line);
			free_ptrarr((void**)params);
			continue;
		}
		printf("params[1]: |%s|\n", params[1]);
		if (ft_strnstr(params[0], "R", ft_strlen(params[0])))
			status = set_resolution(game, params[1], params[2]);
		else if (params[0][0] == 'F' || params[0][0] == 'C')
			status = set_color(game, params[0][0], params[1]);
		else if (params[0][0] == 'S' || !ft_strncmp(params[0], "NO", 3)
			|| !ft_strncmp(params[0], "SO", 3) || !ft_strncmp(params[0], "WE", 3)
			|| !ft_strncmp(params[0], "EA", 3))
			status = load_texture(game, params[0], params[1]);
		else
			status = load_map(game, line);
		free(line);
		line = NULL;
		free_ptrarr((void**)params);
		params = NULL;
	}
	free(line);
	if (status == ERROR || get_pos_from_map(game) || check_map_surrounded(game))
		return (put_and_return_err("Error occured during load map"));
	// print map
	printf("----------------------INPUT MAP---------------------\n");
	for (int i = 0; i < game->map_row; i++)
		printf("%s\n", game->map[i]);

	return (0);
}

