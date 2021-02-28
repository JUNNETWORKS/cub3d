/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 23:48:36 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 23:48:37 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int				add_sprite(t_game *game, double x, double y)
{
	t_vec2	new;
	t_vec2	*new_sprites;

	new.x = x;
	new.y = y;
	if (!(new_sprites = malloc(sizeof(t_vec2) * (game->sprite_num + 1))))
		return (ERROR);
	if (game->sprites)
		ft_memcpy(new_sprites, game->sprites,
			sizeof(t_vec2) * game->sprite_num);
	free(game->sprites);
	new_sprites[game->sprite_num++] = new;
	game->sprites = new_sprites;
	return (0);
}

static void		calc_sprite_dists(t_game *game)
{
	int i;

	i = 0;
	while (i < game->sprite_num)
	{
		game->sprite_dists[i] = (
		(game->player.pos.x - game->sprites[i].x) *
		(game->player.pos.x - game->sprites[i].x) +
		(game->player.pos.y - game->sprites[i].y) *
		(game->player.pos.y - game->sprites[i].y));
		i++;
	}
}

static void		swap_sprite(t_game *game, int a, int b)
{
	double	tmp;
	t_vec2	tmpvec2;

	tmp = game->sprite_dists[a];
	game->sprite_dists[a] = game->sprite_dists[b];
	game->sprite_dists[b] = tmp;
	tmpvec2 = game->sprites[a];
	game->sprites[a] = game->sprites[b];
	game->sprites[b] = tmpvec2;
}

void			sort_sprites(t_game *game)
{
	int		i;
	int		flag;

	calc_sprite_dists(game);
	flag = 1;
	while (flag)
	{
		flag = 0;
		i = game->sprite_num - 1;
		while (i > 0)
		{
			if (game->sprite_dists[i] > game->sprite_dists[i - 1])
			{
				swap_sprite(game, i, i - 1);
				flag = 1;
			}
			i--;
		}
	}
}
