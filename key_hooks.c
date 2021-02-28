/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:14:52 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 18:15:41 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./cub3d.h"

int		key_press_hook(int keycode, t_game *game)
{
	if (keycode == KEY_q || keycode == KEY_esc)
	{
		mlx_destroy_window(game->mlx, game->win);
		exit(0);
	}
	if (keycode == KEY_w)
		game->player.is_moving = 1;
	if (keycode == KEY_s)
		game->player.is_moving = -1;
	if (keycode == KEY_a)
		game->player.is_sidling = -1;
	if (keycode == KEY_d)
		game->player.is_sidling = 1;
	if (keycode == KEY_lallow)
		game->player.is_rotating = -1;
	if (keycode == KEY_rallow)
		game->player.is_rotating = 1;
	return (0);
}

int		key_release_hook(int keycode, t_game *game)
{
	if (keycode == KEY_w || keycode == KEY_s)
		game->player.is_moving = 0;
	if (keycode == KEY_a || keycode == KEY_d)
		game->player.is_sidling = 0;
	if (keycode == KEY_rallow || keycode == KEY_lallow)
		game->player.is_rotating = 0;
	return (0);
}
