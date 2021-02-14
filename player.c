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
		if (game->map[(int)(game->player.pos.y)][(int)new_pos_x] == '0')
			game->player.pos.x = new_pos_x;
			// game->player.pos.x += game->player.is_moving * game->player.dir.x * PLAYER_MOVE_PX;

		double new_pos_y = game->player.pos.y + game->player.is_moving * game->player.dir.y * PLAYER_MOVE_PX;
		new_pos_y = new_pos_y < 0 ? 0 : new_pos_y;
		if (game->map[(int)new_pos_y][(int)(game->player.pos.x)] == '0')
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
		if (game->map[(int)(game->player.pos.y)][(int)new_pos_x] == '0')
			game->player.pos.x = new_pos_x;

		double new_pos_y = game->player.pos.y + game->player.is_sidling * perpendicular.y * PLAYER_MOVE_PX;
		new_pos_y = new_pos_y < 0 ? 0 : new_pos_y;
		if (game->map[(int)new_pos_y][(int)(game->player.pos.x)] == '0')
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
	  player->dir.x = -1;
	  player->dir.y = 0;
	  player->plane.x = 0;
	  player->plane.y = tan(deg2rad(66 / 2));
	}
	if (direction == 'S'){
	  player->dir.x = 1;
	  player->dir.y = 0;
	  player->plane.x = 0;
	  player->plane.y = -1 * tan(deg2rad(66 / 2));
	}
	if (direction == 'E'){
	  player->dir.x = 0;
	  player->dir.y = 1;
	  player->plane.x = tan(deg2rad(66 / 2));
	  player->plane.y = 0;
	}
	if (direction == 'W'){
	  player->dir.x = 0;
	  player->dir.y = -1;
	  player->plane.x = -1 * tan(deg2rad(66 / 2));
	  player->plane.y = 0;
	}
	// 状態の初期化
	player->is_moving = 0;
	player->is_sidling = 0;
	player->is_rotating = 0;
}

/*
void	draw_player(t_game *game)
{
	draw_line_angle_length(game, game->player.position, game->player.angle, 20, 0x00FF0000);

	// ブレゼンハムのアルゴリズムのテスト
	t_vec2 v1_ = {100, 0};
	t_vec2 v2_ = {0, 100};
	draw_2vec2(game, v1_, v2_, 0x000000FF);
	t_vec2 v3_ = {0, 0};
	t_vec2 v4_ = {100, 100};
	draw_2vec2(game, v3_, v4_, 0x0000FFFF);

	// Playerを三角形で描画
	double angle = -1 * game->player.angle;
	t_vec2 player_pos = game->player.pos;
	double length = 20;
	t_vec2 v2 = {player_pos.x + length * cos(angle + 5.0 / 6.0 * M_PI), player_pos.y - length * sin(angle + 5.0 / 6.0 * M_PI)};
	t_vec2 v3 = {player_pos.x + length * cos(angle - 5.0 / 6.0 * M_PI), player_pos.y - length * sin(angle - 5.0 / 6.0 * M_PI)};
	draw_2vec2(game, player_pos, v2, 0x000000FF);
	draw_2vec2(game, v2, v3, 0x0000FF00);
	draw_2vec2(game, v3, player_pos, 0x00FF00FF);

	// 光線(Ray)を作成,描画
	const double ray_length = game->screen_width;
	for (double ray_angle = -M_PI / 2.0; ray_angle <= M_PI / 2.0; ray_angle += M_PI / 1000.0){
		t_vec2 ray_end = {ray_length * cos(angle + ray_angle), -ray_length * sin(angle + ray_angle)};
		vec2_add(&ray_end, player_pos);
		// t_vec2 ray_end = {player_pos.x + ray_length * cos(angle + ray_angle), player_pos.y - ray_length * sin(angle + ray_angle)};
		draw_2vec2(game, player_pos, ray_end, 0x00FFFF00);
	}
}
*/

