/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_player.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:14:03 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 22:50:51 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int			initialize_player(t_player *player,
							double x, double y, char direction)
{
	if (player->pos.x != PLAYER_INIT_POS_X &&
		player->pos.y != PLAYER_INIT_POS_Y)
		return (ERROR);
	player->pos.x = x;
	player->pos.y = y;
	initialize_player_direction(player, direction);
	player->is_moving = 0;
	player->is_sidling = 0;
	player->is_rotating = 0;
	return (0);
}
