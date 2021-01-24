#include "cub3d.h"

char *MAP[] = {
	"1111111111111111111111111",
	"1000000000110000000000001",
	"1011000001110000002000001",
	"100100000000000000000000111111111",
	"111111111011000001110000000000001",
	"100000000011000001110111110111111",
	"11110111111111011100000010001",
	"11110111111111011101010010001",
	"11000000110101011100000010001",
	"10002000000000001100000010001",
	"10000000000000001101010010001",
	"11000001110101011101011010N0111",
	"11110111 1110101 101111010001",
	"11111111 1111111 111111111111",
};

/*
void	move_player(t_game *game)
{
	if (game->player.is_rotating)
	{
		game->player.angle = game->player.angle + game->player.is_rotating * PLAYER_ROTATE_RAD;
		if (game->player.angle >= 2 * M_PI)
			game->player.angle -= 2 * M_PI;
		else if (game->player.angle <= -2 * M_PI)
			game->player.angle += 2 * M_PI;
	}
	if (game->player.is_moving)
	{
		game->player.position.x += game->player.is_moving * PLAYER_MOVE_PX * cos(game->player.angle);
		game->player.position.y += game->player.is_moving * PLAYER_MOVE_PX * sin(game->player.angle);
	}
}

void	draw_wall(t_game *game)
{
	// 各壁ごとに縦横10pxのブロックを描画
	for(int i = 0; i < sizeof(MAP) / sizeof(MAP[0]); i++){
	  for (int j = 0; j < ft_strlen(MAP[i]); j++){
		if (MAP[i][j] == '1'){
			draw_block(game, j * BLOCK_SIZE, i * BLOCK_SIZE, 0x00FFFFFF);
		}
	  }
	}
}

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
	const double ray_length = SCREEN_WIDTH;
	for (double ray_angle = -M_PI / 2.0; ray_angle <= M_PI / 2.0; ray_angle += M_PI / 1000.0){
		t_vec2 ray_end = {ray_length * cos(angle + ray_angle), -ray_length * sin(angle + ray_angle)};
		vec2_add(&ray_end, player_pos);
		// t_vec2 ray_end = {player_pos.x + ray_length * cos(angle + ray_angle), player_pos.y - ray_length * sin(angle + ray_angle)};
		draw_2vec2(game, player_pos, ray_end, 0x00FFFF00);
	}
}
*/

void	initialize_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "Hello world!");
    game->img.img = mlx_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    game->img.addr = mlx_get_data_addr(game->img.img, &game->img.bits_per_pixel, &game->img.line_length, &game->img.endian);
	game->map = MAP;

	// プレイヤーの初期座標
	game->player.pos.x = 0;
	game->player.pos.y = 0;
	// プレイヤーの初期方向
	game->player.dir.x = -1;
	game->player.dir.y = 0;
	// 方向ベクトルに垂直になるようにカメラの平面ベクトルを初期化
	game->player.plane.x = 0;
	game->player.plane.y = 0.66;
	game->player.is_moving = 0;
	game->player.is_rotating = 0;

	// Game Settings
	mlx_do_key_autorepeaton(game->mlx);
}

void	lodev_loop(t_game *game)
{
	// スクリーンの全てのxについて計算する
	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		// カメラ平面上のx座標 (3D表示時の画面のx座標)
		double camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
		t_vec2 ray_dir;
		ray_dir.x = game->player.dir.x + game->player.plane.x * camera_x;
		ray_dir.y = game->player.dir.y + game->player.plane.y * camera_x;
		// map: 現在対象としているマップ内の正方形を表す
		int map_x = (int)game->player.pos.x;
		int map_y = (int)game->player.pos.y;
		// sideDistは, 光線が開始位置から最初の次の正方形に移動するまでの距離
		double side_dist_x;
		double side_dist_y;
		// deltaDistは, 光線が今の正方形から次の正方形に行くために移動する距離
		double delta_dist_x = (1 / ray_dir.x) < 0 ? -(1 / ray_dir.x) : (1 / ray_dir.x);
		double delta_dist_y = (1 / ray_dir.y) < 0 ? -(1 / ray_dir.y) : (1 / ray_dir.y);
		// perpWallDistは, 後に光線の長さを計算する時に使う
		double perp_wall_dist;
		// stepはx,yそれぞれ正か負かどちらの方向に進むか記録する (必ず +1 or -1)
		int step_x;
		int step_y;
		
		// 壁に衝突したか
		int hit = 0;
		// 壁のx面かy面どちらに当たったかを判断するための変数
		int side;

		// stepとsideDistを求める
		if (ray_dir.x < 0)
		{
			step_x = -1;
			side_dist_x = (game->player.pos.x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - game->player.pos.x) * delta_dist_x;
		}
		if (ray_dir.y < 0)
		{
			step_y = -1;
			side_dist_y = (game->player.pos.y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - game->player.pos.y) * delta_dist_y;
		}

		// 光線が壁にぶつかるまで光線を進める
		while (hit == 0)
		{
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			// 光線が壁にぶつかったか確認する
			if (game->map[map_x][map_y] > 0)
				hit = 1;
		}

		// 壁までの光線の距離を計算する
		if (side == 0)
			perp_wall_dist = (map_x - game->player.pos.x + (1 - step_x) / 2) / ray_dir.x;
		else
			perp_wall_dist = (map_y - game->player.pos.y + (1 - step_y) / 2) / ray_dir.y;

		// スクリーンに描画する必要のある縦線の長さを求める
		int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
		// 実際に描画すべき場所の開始位置と終了位置を計算
		int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_end >= SCREEN_HEIGHT)
			draw_end = SCREEN_HEIGHT - 1;

		int color;
		if (side == 0)
			color = 0x00FFFFFF;
		else
			color = 0x00888888;

		t_vec2 v_start = {x, draw_start};
		t_vec2 v_end = {x, draw_end};
		draw_2vec2(game, v_start, v_end, color);
	}
}

int		main_loop(t_game *game)
{
	clear_img(game);
	lodev_loop(game);
	/*
	draw_wall(game);
	draw_player(game);
	print_game(game);
	move_player(game);
	*/
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int main(int argc, char **argv){
	t_game	game;
	initialize_game(&game);

	mlx_hook(game.win, KeyPress, KeyPressMask, key_press_hook, &game);
	mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release_hook, &game);
	mlx_loop_hook(game.mlx, &main_loop, &game);
    mlx_loop(game.mlx);
}
