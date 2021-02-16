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

	if ((fd = open(filepath, O_RDONLY)) == -1)
		return (-1);
	while ((status = get_next_line(fd, &line)) == 1)
	{
		params = ft_split(line, ' ');
		free(line);

		if (ft_strnstr(params[0], "R", ft_strlen(params[0])) == 0){
			game->screen_width = ft_atoi(params[1]);
			game->screen_height = ft_atoi(params[2]);
		}else if (ft_strnstr(params[0], "NO", ft_strlen(params[0])) == 0){
			load_image(game, &(game->tex_n), params[1]);
		}else if (ft_strnstr(params[0], "SO", ft_strlen(params[0])) == 0){
			load_image(game, &(game->tex_s), params[1]);
		}else if (ft_strnstr(params[0], "WE", ft_strlen(params[0])) == 0){
			load_image(game, &(game->tex_w), params[1]);
		}else if (ft_strnstr(params[0], "EA", ft_strlen(params[0])) == 0){
			load_image(game, &(game->tex_e), params[1]);
		}else if (ft_strnstr(params[0], "S", ft_strlen(params[0])) == 0){
			load_image(game, &(game->tex_sprite), params[1]);
		}else if (ft_strnstr(params[0], "F", ft_strlen(params[0])) == 0){
			params2 = ft_split(params[1], ',');
			game->ground_color = rgb2hex(ft_atoi(params2[0]), ft_atoi(params2[1]), ft_atoi(params2[2]));
			free_ptrarr((void**)params2);
		}else if (ft_strnstr(params[0], "C", ft_strlen(params[0])) == 0){
			params2 = ft_split(params[1], ',');
			game->sky_color = rgb2hex(ft_atoi(params2[0]), ft_atoi(params2[1]), ft_atoi(params2[2]));
			free_ptrarr((void**)params2);
		}else{
			// マップの読み込み
		}
		free_ptrarr((void**)params);
	}
	free(line);
	return (status);
}

