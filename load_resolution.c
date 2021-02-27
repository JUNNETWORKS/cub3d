#include "cub3d.h"

int		set_resolution(t_game *game, char *width_str, char *height_str)
{
	int	width;
	int	height;
	int	max_width;
	int	max_height;

	printf("width_str: %s, height_str: %s\n", width_str, height_str);
	if (game->screen_width || game->screen_height)
		return (put_and_return_err("Resolution has already configured"));
	if (!str_all_true(width_str, ft_isdigit) ||
		!str_all_true(height_str, ft_isdigit) ||
		(width = ft_atoi(width_str)) <= 0 ||
		(height = ft_atoi(height_str)) <= 0 ||
		(width_str[0] == '0' && width) ||
		(height_str[0] == '0' && height))
		return (put_and_return_err("Resolution is invalid"));
	mlx_get_screen_size(game->mlx, &max_width, &max_height);
	printf("Display size\n\twidth: %d\n\theight: %d\n", max_width, max_height);
	game->screen_width = width > max_width ? max_width : width;
	game->screen_height = height > max_height ? max_height : height;
	return (0);
}

