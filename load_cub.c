#include "./cub3d.h"

void	free_ptrarr(void **ptrarr)
{
	size_t i;

	i = 0;
	while (ptrarr[i])
	{
		free(ptrarr[i]);
		ptrarr[i] = NULL;
		i++;
	}
}

// returun value is false if floodfill hit outside of map
bool floodfill(t_game *game, bool **filled_map, int i, int j)
{
	if (i < 0 || i >= game->map_row || j < 0 || j >= game->map_col)
		return (false);
	if (game->map[i][j] == '1' || filled_map[i][j] == true)
		return (true);
	else
		filled_map[i][j] = true;
	bool is_surrounded = true;
	is_surrounded &= floodfill(game, filled_map, i - 1, j);
	is_surrounded &= floodfill(game, filled_map, i + 1, j);
	is_surrounded &= floodfill(game, filled_map, i, j - 1);
	is_surrounded &= floodfill(game, filled_map, i, j + 1);
	return (is_surrounded);
}

// determine whether the map is surrounded or not by floodfill algorithm
int	check_map_surrounded(t_game *game)
{
	int	x;
	int	y;

	x = game->player.pos.x;
	y = game->player.pos.y;

	bool	**filled_map = ft_calloc(game->map_row, sizeof(bool*));
	for (int i = 0; i < game->map_row; i++)
	  filled_map[i] = ft_calloc(game->map_col, sizeof(bool));

	// floodfill
	bool is_surrounded = floodfill(game, filled_map, y, x);

	// print floodfill result
	printf("------------------floodfill result--------------------\n");
	printf("result: %s\n", is_surrounded ? "is_surrounded" : "is_not_surrounded");
	for (int i = 0; i < game->map_row; i++){
	  printf("|");
	  for (int j = 0; j < game->map_col; j++){
		printf("%c ", filled_map[i][j] ? 'X' : ' ');
	  }
	  printf("|\n");
	}

	return (0);
}

void add_sprite(t_game *game, t_vec2 new)
{
	t_vec2 *new_sprites = malloc(sizeof(t_vec2) * (game->sprite_num + 1));
	if (game->sprites)
		ft_memcpy(new_sprites, game->sprites, sizeof(t_vec2) * game->sprite_num);
	free(game->sprites);
	new_sprites[game->sprite_num++] = new;
	game->sprites = new_sprites;
}

// get player and sprite positions from map
int	get_pos_from_map(t_game *game)
{
	// この辺の初期化は別の場所でやるべき
	game->sprite_num = 0;
	game->sprites = NULL;
	game->player.pos.x = -1;
	game->player.pos.y = -1;

	for (int i = 0; i < game->map_row; i++){
		for (int j = 0; j < game->map_col; j++){
			if (game->map[i][j] == '\0')
				continue;
			if (game->map[i][j] == '2'){
				t_vec2 sprite;
				sprite.x = j + 0.5;
				sprite.y = i + 0.5;
				printf("add sprite {x: %lf, y: %lf}\n", sprite.x, sprite.y);
				add_sprite(game, sprite);
			}
			else if (ft_strchr("NSWE", game->map[i][j])){
				if (game->player.pos.x >= 0 && game->player.pos.y >= 0){
					printf("プレイヤー2人目なんですけどww?\n");
					printf("map[%d]: %s\n", i, game->map[i]);
					printf("map[%d][%d]: %c (%#x)\n", i, j, game->map[i][j], game->map[i][j]);
					return (-1);
				}
				initialize_player(&game->player, j + 0.5, i + 0.5, game->map[i][j]);
			}
		}
	}
	// print sprites
	printf("-----------------------SPRITES--------------------\n");
	for (int i = 0; i < game->sprite_num; i++)
	  printf("sprites[%d] = {x: %lf, y: %lf}\n", i, game->sprites[i].x, game->sprites[i].y);
	return (0);
}


int	load_cubfile(t_game *game, char *filepath)
{
	int		fd;
	char	*line;
	int		status;
	char	**params;
	char	**params2;
	int		map_row = 0;
	int		map_col = 0;
	game->map = ft_calloc(MAX_MAP_HEIGHT, sizeof(char*));  // 200 * 200が最大MAPサイズ

	if ((fd = open(filepath, O_RDONLY)) == -1)
		return (-1);
	while ((status = get_next_line(fd, &line)) == 1)
	{
		params = ft_split(line, ' ');

		printf("params[0]: |%s|\n", params[0]);
		if (params[0] == NULL)
			continue;
		printf("params[1]: |%s|\n", params[1]);
		if (ft_strnstr(params[0], "R", ft_strlen(params[0]))){
			game->screen_width = ft_atoi(params[1]);
			game->screen_height = ft_atoi(params[2]);
			printf("screen_width: %d, screen_height: %d\n",game->screen_width, game->screen_height);
		}else if (ft_strnstr(params[0], "NO", ft_strlen(params[0]))){
			if(load_image(game, &(game->tex_n), params[1]) == -1){
				printf("load error %s\n", params[1]);
				return (-1);
			}
		}else if (ft_strnstr(params[0], "SO", ft_strlen(params[0]))){
			if(load_image(game, &(game->tex_s), params[1]) == -1){
				printf("load error %s\n", params[1]);
				return (-1);
			}
		}else if (ft_strnstr(params[0], "WE", ft_strlen(params[0]))){
			if(load_image(game, &(game->tex_w), params[1]) == -1){
				printf("load error %s\n", params[1]);
				return (-1);
			}
		}else if (ft_strnstr(params[0], "EA", ft_strlen(params[0]))){
			if(load_image(game, &(game->tex_e), params[1]) == -1){
				printf("load error %s\n", params[1]);
				return (-1);
			}
		}else if (ft_strnstr(params[0], "S", ft_strlen(params[0]))){
			if(load_image(game, &(game->tex_sprite), params[1]) == -1){
				printf("load error %s\n", params[1]);
				return (-1);
			}
		}else if (ft_strnstr(params[0], "F", ft_strlen(params[0]))){
			params2 = ft_split(params[1], ',');
			game->ground_color = rgb2hex(ft_atoi(params2[0]), ft_atoi(params2[1]), ft_atoi(params2[2]));
			free_ptrarr((void**)params2);
		}else if (ft_strnstr(params[0], "C", ft_strlen(params[0]))){
			params2 = ft_split(params[1], ',');
			game->sky_color = rgb2hex(ft_atoi(params2[0]), ft_atoi(params2[1]), ft_atoi(params2[2]));
			free_ptrarr((void**)params2);
		}else{
			if (!line || ft_strlen(line) >= MAX_MAP_WIDTH || map_row >= MAX_MAP_WIDTH){
				printf("error occured during load map\n");
				continue;
			}
			game->map[map_row] = ft_calloc(MAX_MAP_WIDTH, sizeof(char));
			ft_strlcpy(game->map[map_row], line, ft_strlen(line) + 1);
			if (!game->map[map_row])
				printf("error strdup()\n");
			map_row++;
			map_col = ft_strlen(line) > map_col ? ft_strlen(line) : map_col;
		}
		free(line);
		free_ptrarr((void**)params);
	}
	free(line);
	game->map_row = map_row;
	game->map_col = map_col;

	// print map
	printf("----------------------INPUT MAP---------------------\n");
	for (int i = 0; i < game->map_row; i++)
		printf("%s\n", game->map[i]);

	get_pos_from_map(game);

	check_map_surrounded(game);

	return (status);
}

