#include "./cub3d.h"

size_t	ptrarr_len(void **ptrarr)
{
	size_t i;

	i = 0;
	while (ptrarr[i])
		i++;
	return (i);
}

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
	free(ptrarr);
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

	// +1 はNULL終端用
	bool	**filled_map = ft_calloc(game->map_row + 1, sizeof(bool*));
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
	free_ptrarr((void**)filled_map);
	if (!is_surrounded)
		return (put_and_return_err("Map must be surrounded by wall"));
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
				if (game->player.pos.x >= 0 && game->player.pos.y >= 0)
					return (put_and_return_err("Player's position must be unique"));
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

int	load_texture(t_game *game, char *name, char *texture_path)
{
	t_img *tex;
	printf("name: %s, texture_path: %s\n", name, texture_path);
	if (ft_strnstr(name, "NO", ft_strlen(name))){
		tex = &game->tex_n;
	}else if (ft_strnstr(name, "SO", ft_strlen(name))){
		tex = &game->tex_s;
	}else if (ft_strnstr(name, "WE", ft_strlen(name))){
		tex = &game->tex_w;
	}else if (ft_strnstr(name, "EA", ft_strlen(name))){
		tex = &game->tex_e;
	}else if (name[0] == 'S'){
		tex = &game->tex_sprite;
	}
	if(load_image(game, tex, texture_path) == -1){
		put_error_msg("Failed to load texture");
		return (-1);
	}
	return (0);
}

int	set_color(t_game *game, char name, char *rgbstr)
{
	char **rgb = ft_split(rgbstr, ',');
	if (ptrarr_len((void**)rgb) != 3){
		free_ptrarr((void**)rgb);
		return (-1);
	}
	int r = ft_atoi(rgb[0]);
	int g = ft_atoi(rgb[1]);
	int b = ft_atoi(rgb[2]);
	free_ptrarr((void**)rgb);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (put_and_return_err("provided color is invalid"));
	if (name == 'F')
		game->ground_color = rgb2hex(r, g, b);
	else if (name == 'C')
		game->sky_color = rgb2hex(r, g, b);
	else
		return (put_and_return_err("Unknow key is provided"));
	return (0);
}

int	load_map(t_game *game, char *line)
{
	if (!line || ft_strlen(line) >= MAX_MAP_WIDTH || game->map_row >= MAX_MAP_WIDTH){
		put_error_msg("map is too large");
		return (-1);
	}
	game->map[game->map_row] = ft_calloc(MAX_MAP_WIDTH, sizeof(char));
	ft_strlcpy(game->map[game->map_row], line, ft_strlen(line) + 1);
	if (!game->map[game->map_row]){
		put_error_msg("error strdup()");
		return (-1);
	}
	game->map_row++;
	game->map_col = ft_strlen(line) > game->map_col ? ft_strlen(line) : game->map_col;
	return (0);
}

int	set_resolution(t_game *game, int width, int height)
{
	game->screen_width = width;
	game->screen_height = height;
	return (0);
}

int	load_cubfile(t_game *game, char *filepath)
{
	int		fd;
	char	*line;
	int		status;
	char	**params;
	game->map = ft_calloc(MAX_MAP_HEIGHT, sizeof(char*));  // 200 * 200が最大MAPサイズ
	game->map_row = 0;
	game->map_col = 0;

	if ((fd = open(filepath, O_RDONLY)) == -1)
		return (-1);
	status = 1;
	while (status >= 0 && (status = get_next_line(fd, &line)) == 1)
	{
		params = ft_split(line, ' ');

		printf("params[0]: |%s|\n", params[0]);
		if (params[0] == NULL){
			free(line);
			free_ptrarr((void**)params);
			continue;
		}
		printf("params[1]: |%s|\n", params[1]);
		if (ft_strnstr(params[0], "R", ft_strlen(params[0]))){
			set_resolution(game, ft_atoi(params[1]), ft_atoi(params[2]));
			printf("screen_width: %d, screen_height: %d\n",game->screen_width, game->screen_height);
		}else if (params[0][0] == 'F' || params[0][0] == 'C'){
			status = set_color(game, params[0][0], params[1]);
		}else if (params[0][0] == 'S' || !ft_strncmp(params[0], "NO", 3)
			|| !ft_strncmp(params[0], "SO", 3) || !ft_strncmp(params[0], "WE", 3)
			|| !ft_strncmp(params[0], "EA", 3)){
			status = load_texture(game, params[0], params[1]);
		}else{
			load_map(game, line);
		}
		free(line);
		free_ptrarr((void**)params);
		params = NULL;
	}
	free(line);

	// print map
	printf("----------------------INPUT MAP---------------------\n");
	for (int i = 0; i < game->map_row; i++)
		printf("%s\n", game->map[i]);

	get_pos_from_map(game);

	check_map_surrounded(game);

	return (status);
}

