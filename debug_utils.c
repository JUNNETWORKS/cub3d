#include "cub3d.h"

void	print_game(t_game *game)
{
	printf("\n-------------game-------------\n");
	printf("player\n\tposition\n\t\tx: %d\n\t\ty: %d\n\tangle: %lf\n\tstatus\n\t\tis_rotating: %d\n\t\tis_moving: %d\n", game->player.position.x, game->player.position.y, game->player.angle, game->player.is_rotating, game->player.is_moving);
}
