#include "./cub3d.h"

void	move_player(t_game *game)
{
	if (game->player.is_rotating)
	{
		double rot_speed = game->player.is_rotating * PLAYER_ROTATE_RAD;

		t_vec2 old_dir = game->player.dir;
		game->player.dir.x = old_dir.x * cos(rot_speed) - old_dir.y * sin(rot_speed);
		game->player.dir.y = old_dir.x * sin(rot_speed) + old_dir.y * cos(rot_speed);

		t_vec2 old_plane = game->player.plane;
		game->player.plane.x = game->player.plane.x * cos(rot_speed) - game->player.plane.y * sin(rot_speed);
		game->player.plane.y = old_plane.x * sin(rot_speed) + game->player.plane.y * cos(rot_speed);
	}
	if (game->player.is_moving)
	{
		double new_pos_x = game->player.pos.x + game->player.is_moving * game->player.dir.x * PLAYER_MOVE_PX;
		new_pos_x = new_pos_x < 0 ? 0 : new_pos_x;
		if (game->map[(int)(game->player.pos.y)][(int)new_pos_x] != '1')
			game->player.pos.x = new_pos_x;
			// game->player.pos.x += game->player.is_moving * game->player.dir.x * PLAYER_MOVE_PX;

		double new_pos_y = game->player.pos.y + game->player.is_moving * game->player.dir.y * PLAYER_MOVE_PX;
		new_pos_y = new_pos_y < 0 ? 0 : new_pos_y;
		if (game->map[(int)new_pos_y][(int)(game->player.pos.x)] != '1')
			game->player.pos.y = new_pos_y;
			// game->player.pos.y += game->player.is_moving * game->player.dir.y * PLAYER_MOVE_PX;
	}
	if (game->player.is_sidling)
	{
		t_vec2 perpendicular;
		perpendicular.x = game->player.dir.x * cos(M_PI/2) - game->player.dir.y * sin(M_PI/2);
		perpendicular.y = game->player.dir.x * sin(M_PI/2) + game->player.dir.y * cos(M_PI/2);
		double new_pos_x = game->player.pos.x + game->player.is_sidling * perpendicular.x * PLAYER_MOVE_PX;
		new_pos_x = new_pos_x < 0 ? 0 : new_pos_x;
		if (game->map[(int)(game->player.pos.y)][(int)new_pos_x] != '1')
			game->player.pos.x = new_pos_x;

		double new_pos_y = game->player.pos.y + game->player.is_sidling * perpendicular.y * PLAYER_MOVE_PX;
		new_pos_y = new_pos_y < 0 ? 0 : new_pos_y;
		if (game->map[(int)new_pos_y][(int)(game->player.pos.x)] != '1')
			game->player.pos.y = new_pos_y;
	}
}

void initialize_player(t_player *player, double x, double y, char direction)
{
	// プレイヤーの初期座標
	player->pos.x = x;
	player->pos.y = y;
	/*
	// プレイヤーの初期方向  (長さが1の正規化されたベクトルにする必要がある)
	player->dir.x = -1;
	player->dir.y = 0;
	// 方向ベクトルに垂直になるようにカメラの平面ベクトルを初期化
	player->plane.x = 0;
	// player->plane.y = 0.66;
	player->plane.y = tan(deg2rad(66 / 2));
	*/
	if (direction == 'N'){
	  player->dir.x = 0;
	  player->dir.y = -1;
	  player->plane.x = 1 * tan(deg2rad(66 / 2));
	  player->plane.y = 0;
	}
	if (direction == 'S'){
	  player->dir.x = 0;
	  player->dir.y = 1;
	  player->plane.x = -1 * tan(deg2rad(66 / 2));
	  player->plane.y = 0;
	}
	if (direction == 'E'){
	  player->dir.x = 1;
	  player->dir.y = 0;
	  player->plane.x = 0;
	  player->plane.y = 1 * tan(deg2rad(66 / 2));
	}
	if (direction == 'W'){
	  player->dir.x = -1;
	  player->dir.y = 0;
	  player->plane.x = 0;
	  player->plane.y = -1 * tan(deg2rad(66 / 2));
	}
	// 状態の初期化
	player->is_moving = 0;
	player->is_sidling = 0;
	player->is_rotating = 0;
}

