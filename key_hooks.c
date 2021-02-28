/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:14:52 by jtanaka           #+#    #+#             */
/*   Updated: 2021/03/01 00:28:51 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

int		key_press_hook(int keycode, t_game *game)
{
	if (keycode == KEY_Q || keycode == KEY_ESC)
	{
		mlx_destroy_window(game->mlx, game->win);
		exit(0);
	}
	if (keycode == KEY_W)
		game->player.is_moving = 1;
	if (keycode == KEY_S)
		game->player.is_moving = -1;
	if (keycode == KEY_A)
		game->player.is_sidling = -1;
	if (keycode == KEY_D)
		game->player.is_sidling = 1;
	if (keycode == KEY_L_ARROW)
		game->player.is_rotating = -1;
	if (keycode == KEY_R_ARROW)
		game->player.is_rotating = 1;
	return (0);
}

int		key_release_hook(int keycode, t_game *game)
{
	if (keycode == KEY_W || keycode == KEY_S)
		game->player.is_moving = 0;
	if (keycode == KEY_A || keycode == KEY_D)
		game->player.is_sidling = 0;
	if (keycode == KEY_R_ARROW || keycode == KEY_L_ARROW)
		game->player.is_rotating = 0;
	return (0);
}
