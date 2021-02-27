#include "cub3d.h"

void		initialize_player_direction(t_player *player, char direction)
{
	if (direction == 'N' || direction == 'S')
	{
		player->dir.x = 0;
		player->dir.y = direction == 'N' ? -1 : 1;
		player->plane.x = (direction == 'N' ? 1 : -1) * tan(deg2rad(66 / 2));
		player->plane.y = 0;
	}
	if (direction == 'E' || direction == 'W')
	{
		player->dir.x = direction == 'E' ? 1 : -1;
		player->dir.y = 0;
		player->plane.x = 0;
		player->plane.y = (direction == 'E' ? 1 : -1) * tan(deg2rad(66 / 2));
	}
}

void		initialize_player(t_player *player, double x, double y, char direction)
{
	player->pos.x = x;
	player->pos.y = y;
	initialize_player_direction(player, direction);
	player->is_moving = 0;
	player->is_sidling = 0;
	player->is_rotating = 0;
}

