#include "cub3d.h"

void	print_game(t_game *game)
{
	printf("\n-------------game-------------\n");
	printf("player\n");
	printf("pos\n\tx: %lf\n\ty: %lf\n", game->player.pos.x, game->player.pos.y);
	printf("dir\n\tx: %lf\n\ty: %lf\n", game->player.dir.x, game->player.dir.y);
	printf("plane\n\tx: %lf\n\ty: %lf\n", game->player.plane.x, game->player.plane.y);
	printf("status\n\tis_moving: %d\n\tis_rotating: %d\n", game->player.is_moving, game->player.is_rotating);
}
