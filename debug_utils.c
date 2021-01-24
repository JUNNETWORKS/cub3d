#include "cub3d.h"

void	print_game(t_game *game)
{
	printf("\n-------------game-------------\n");
	printf("player\n\tpos\n\t\tx: %lf\n\t\ty: %lf\n\tstatus\n\t\tis_rotating: %d\n\t\tis_moving: %d\n", game->player.pos.x, game->player.pos.y, game->player.is_rotating, game->player.is_moving);
}
