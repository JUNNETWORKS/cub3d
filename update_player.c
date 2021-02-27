#include "./cub3d.h"

static void	rotate_player(t_game *game)
{
	double	rot_speed;

	if (game->player.is_rotating)
	{
		rot_speed = game->player.is_rotating * PLAYER_ROTATE_RAD;

		vec2_rotate(&game->player.dir, rot_speed);
		vec2_rotate(&game->player.plane, rot_speed);
	}
}

static void	move_player(t_game *game)
{
	double	new_pos_x;
	double	new_pos_y;

	if (game->player.is_moving)
	{
		new_pos_x = game->player.pos.x + game->player.is_moving *
		  game->player.dir.x * PLAYER_MOVE_PX;
		new_pos_x = new_pos_x < 0 ? 0 : new_pos_x;
		if (game->map[(int)(game->player.pos.y)][(int)new_pos_x] != '1')
			game->player.pos.x = new_pos_x;

		new_pos_y = game->player.pos.y + game->player.is_moving *
		  game->player.dir.y * PLAYER_MOVE_PX;
		new_pos_y = new_pos_y < 0 ? 0 : new_pos_y;
		if (game->map[(int)new_pos_y][(int)(game->player.pos.x)] != '1')
			game->player.pos.y = new_pos_y;
	}
}

static void	slide_player(t_game *game)
{
	double	new_pos_x;
	double	new_pos_y;
	t_vec2	perpendicular;

	if (game->player.is_sidling)
	{
		perpendicular = game->player.dir;
		vec2_rotate(&perpendicular, M_PI / 2);
		new_pos_x = game->player.pos.x + game->player.is_sidling * perpendicular.x * PLAYER_MOVE_PX;
		new_pos_x = new_pos_x < 0 ? 0 : new_pos_x;
		if (game->map[(int)(game->player.pos.y)][(int)new_pos_x] != '1')
			game->player.pos.x = new_pos_x;

		new_pos_y = game->player.pos.y + game->player.is_sidling * perpendicular.y * PLAYER_MOVE_PX;
		new_pos_y = new_pos_y < 0 ? 0 : new_pos_y;
		if (game->map[(int)new_pos_y][(int)(game->player.pos.x)] != '1')
			game->player.pos.y = new_pos_y;
	}
}

void		update_player(t_game *game)
{
	rotate_player(game);
	move_player(game);
	slide_player(game);
}
