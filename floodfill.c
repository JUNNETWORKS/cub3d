#include "cub3d.h"

// returun value is false if floodfill hit outside of map
static bool	floodfill(t_game *game, bool **filled_map, int i, int j)
{
	bool is_surrounded;

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

// determine whether the map is surrounded or not by floodfill algorithm
int			check_map_surrounded(t_game *game)
{
	int		x;
	int		y;
	bool	**filled_map;
	bool	is_surrounded;

	x = game->player.pos.x;
	y = game->player.pos.y;

	// +1 はNULL終端用
	filled_map = ft_calloc(game->map_row + 1, sizeof(bool*));
	y = 0;
	while (y < game->map_row)
		filled_map[y++] = ft_calloc(game->map_col, sizeof(bool));

	// floodfill
	is_surrounded = floodfill(game, filled_map, y, x);

	// print floodfill result
	printf("------------------floodfill result--------------------\n");
	printf("result: %s\n", is_surrounded ? "is_surrounded" : "is_not_surrounded");
	for (int y = 0; y < game->map_row; y++){
	  printf("|");
	  for (int x = 0; x < game->map_col; x++){
		printf("%c ", filled_map[y][x] ? 'X' : ' ');
	  }
	  printf("|\n");
	}
	free_ptrarr((void**)filled_map);
	if (!is_surrounded)
		return (put_and_return_err("Map isn't surrounded by wall"));
	return (0);
}

