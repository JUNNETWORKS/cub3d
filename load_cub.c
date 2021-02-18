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
			ft_strlcpy(game->map[map_row], line, ft_strlen(line));
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

	return (status);
}

