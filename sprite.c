#include "cub3d.h"

int		add_sprite(t_game *game, double x, double y)
{
	t_vec2	new;
	t_vec2	*new_sprites;

	new.x = x;
	new.y = y;
	if (!(new_sprites = malloc(sizeof(t_vec2) * (game->sprite_num + 1))))
		return (ERROR);
	if (game->sprites)
		ft_memcpy(new_sprites, game->sprites, sizeof(t_vec2) * game->sprite_num);
	free(game->sprites);
	new_sprites[game->sprite_num++] = new;
	game->sprites = new_sprites;
	return (0);
}

static void	calc_sprite_dists(t_game *game, double *sprite_distances)
{
	int i;

	i = 0;
	while (i < game->sprite_num){
		sprite_distances[i] = (
		(game->player.pos.x - game->sprites[i].x) *
		(game->player.pos.x - game->sprites[i].x) +
		(game->player.pos.y - game->sprites[i].y) *
		(game->player.pos.y - game->sprites[i].y));
		i++;
	}
}

int			sort_sprites(t_game *game)
{
	double	*sprite_dists;
	int		i;
	int		flag;
	double	tmp;
	t_vec2	tmpvec2;

	if (!(sprite_dists = ft_calloc(game->sprite_num, sizeof(double))))
		return (put_and_return_err("malloc failed"));
	calc_sprite_dists(game, sprite_dists);
	flag = 1;
	while (flag){
		flag = 0;
		i = game->sprite_num - 1;
		while (i > 0){
			if (sprite_dists[i] > sprite_dists[i-1]){
				tmp = sprite_dists[i];
				sprite_dists[i] = sprite_dists[i-1];
				sprite_dists[i-1] = tmp;
				tmpvec2 = game->sprites[i];
				game->sprites[i] = game->sprites[i-1];
				game->sprites[i-1] = tmpvec2;
				flag = 1;
			}
			i--;
		}
	}
	free(sprite_dists);
	return (0);
}
