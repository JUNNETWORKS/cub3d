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

static void	calc_sprite_dists(t_game *game)
{
	int i;

	i = 0;
	while (i < game->sprite_num){
		game->sprite_dists[i] = (
		(game->player.pos.x - game->sprites[i].x) *
		(game->player.pos.x - game->sprites[i].x) +
		(game->player.pos.y - game->sprites[i].y) *
		(game->player.pos.y - game->sprites[i].y));
		i++;
	}
}

void			sort_sprites(t_game *game)
{
	int		i;
	int		flag;
	double	tmp;
	t_vec2	tmpvec2;

	calc_sprite_dists(game);
	flag = 1;
	while (flag){
		flag = 0;
		i = game->sprite_num - 1;
		while (i > 0){
			if (game->sprite_dists[i] > game->sprite_dists[i-1]){
				tmp = game->sprite_dists[i];
				game->sprite_dists[i] = game->sprite_dists[i-1];
				game->sprite_dists[i-1] = tmp;
				tmpvec2 = game->sprites[i];
				game->sprites[i] = game->sprites[i-1];
				game->sprites[i-1] = tmpvec2;
				flag = 1;
			}
			i--;
		}
	}
}
