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

void	sort_sprites(t_game *game)
{
	double	*sprite_distances;
	int		i;
	int		flag;
	double	tmp;
	// スプライトのソートで使う(スプライトまでの距離)
	sprite_distances = ft_calloc(game->sprite_num, sizeof(double));

	// スプライトを遠い順にソートするために距離を求める
	for (i = 0; i < game->sprite_num; i++){
	  sprite_distances[i] = ((game->player.pos.x - game->sprites[i].x) * (game->player.pos.x - game->sprites[i].x) + (game->player.pos.y - game->sprites[i].y) * (game->player.pos.y - game->sprites[i].y));
	}

	// 遠い順にスプライトが並ぶようにソート
	// バブルソート
	flag = 1;
	while (flag){
		flag = 0;
		for (i = game->sprite_num - 1; i > 0; i--){
			if (sprite_distances[i] > sprite_distances[i-1]){
				tmp = sprite_distances[i];
				sprite_distances[i] = sprite_distances[i-1];
				sprite_distances[i-1] = tmp;

				t_vec2 tmpvec2 = game->sprites[i];
				game->sprites[i] = game->sprites[i-1];
				game->sprites[i-1] = tmpvec2;

				flag = 1;
			}
		}
	}
	free(sprite_distances);
}
