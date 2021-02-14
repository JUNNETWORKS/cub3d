#include "cub3d.h"

char *MAP[] = {
	"1111111111111111111111111",
	"1000000000110000000000001",
	"1000000001110000001000001",
	"100100000000000000000000111111111",
	"111111111011000001110000000000001",
	"100000000011000001110111110111111",
	"11110111111111011100000010001",
	"11110111111111011101010010001",
	"11000000110101011100000010001",
	"10001000000000001100000010001",
	"10000000000000001101010010001",
	"1100000111010101110101101000111",
	"11110111 1110101 101111010001",
	"11111111 1111111 111111111111",
};

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

void	initialize_game(t_game *game)
{
    game->mlx = mlx_init();
	// 640 * 480 以外だと壁が縦長や横長になる
	game->screen_width = 640;
	game->screen_height = 480;
    game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "Hello world!");
    game->img.img = mlx_new_image(game->mlx, game->screen_width, game->screen_height);
    game->img.addr = mlx_get_data_addr(game->img.img, &game->img.bits_per_pixel, &game->img.line_length, &game->img.endian);
	game->map = MAP;

	// XPMファイルからテクスチャ画像を読み込む
	load_image(game, &(game->tex_n), "./textures/wall_n.xpm");
	game->tex_width = game->tex_n.width;
	game->tex_height = game->tex_n.height;
	load_image(game, &(game->tex_s), "./textures/wall_s.xpm");
	load_image(game, &(game->tex_w), "./textures/wall_w.xpm");
	load_image(game, &(game->tex_e), "./textures/wall_e.xpm");

	// プレイヤーの初期座標
	game->player.pos.x = 2.0;
	game->player.pos.y = 1.5;
	// プレイヤーの初期方向  (長さが1の正規化されたベクトルにする必要がある)
	game->player.dir.x = -1;
	game->player.dir.y = 0;
	// 方向ベクトルに垂直になるようにカメラの平面ベクトルを初期化
	game->player.plane.x = 0;
	// game->player.plane.y = 0.66;
	game->player.plane.y = tan(deg2rad(66 / 2));
	// 状態の初期化
	game->player.is_moving = 0;
	game->player.is_sidling = 0;
	game->player.is_rotating = 0;

	// スプライト用
	char *sprite_texture_path = "./textures/barrel.xpm";
	game->tex_sprite.img = mlx_xpm_file_to_image(game->mlx, sprite_texture_path, &game->sprite_width, &game->sprite_height);
    game->tex_sprite.addr = mlx_get_data_addr(game->tex_sprite.img, &game->tex_sprite.bits_per_pixel, &game->tex_sprite.line_length, &game->tex_sprite.endian);
	game->z_buffer = ft_calloc(game->screen_width, sizeof(double));
	game->sprite_num = 1;
	game->sprites = ft_calloc(game->sprite_num, sizeof(t_vec2));
	game->sprites[0].x = 1.5;
	game->sprites[0].y = 1.5;
	// game->sprites[1].x = 2.5;
	// game->sprites[1].y = 2.5;
	
	// Game Settings
	mlx_do_key_autorepeaton(game->mlx);
}

void	sort_sprites(t_game *game)
{
	// スプライトのソートで使う(スプライト番号)
	// int sprite_order[SPRITES_NUM];
	
	// スプライトのソートで使う(スプライトまでの距離)
	double *sprite_distances = ft_calloc(game->sprite_num, sizeof(double));

	// スプライトを遠い順にソートするために距離を求める
	for (int i = 0; i < game->sprite_num; i++){
	  sprite_distances[i] = ((game->player.pos.x - game->sprites[i].x) * (game->player.pos.x - game->sprites[i].x) + (game->player.pos.y - game->sprites[i].y) * (game->player.pos.y - game->sprites[i].y));
	}

	// 遠い順にスプライトが並ぶようにソート
	// バブルソート
	int flag = 1;
	while (flag){
		flag = 0;
		for (int i = game->sprite_num - 1; i > 0; i--){
			if (sprite_distances[i] > sprite_distances[i-1]){
				double tmp = sprite_distances[i];
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

void	lodev_loop(t_game *game)
{
	// planeベクトルの大きさを計算
	double	plane_length = vec2_length(game->player.plane);
	// 基準となる壁の高さ. 視野角に応じて横幅が変わってしまうので, 視野角の逆数を掛けて1に戻す
	double	wall_height_base = (double)game->screen_width * (1 / (2 * plane_length));
	// スクリーンの全てのxについて計算する
	for (int x = 0; x < game->screen_width; x++)
	{
		// カメラ平面上のx座標 (3D表示時の画面のx座標)  -1.0~1.0
		double camera_x = 2 * x / (double)game->screen_width - 1;
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
		double delta_dist_x = (1 / ray_dir.x) < 0 ? -1 * (1 / ray_dir.x) : (1 / ray_dir.x);
		double delta_dist_y = (1 / ray_dir.y) < 0 ? -1 * (1 / ray_dir.y) : (1 / ray_dir.y);
		// perpWallDistは, 当たった壁とカメラ平面ベクトルとの距離を表す
		double perp_wall_dist;
		// stepはx,yそれぞれ正か負かどちらの方向に進むか記録する (必ず +1 or -1)
		int step_x;
		int step_y;
		
		// 壁に衝突したか
		int hit = 0;
		// 壁のx面かy面どちらに当たったかを判断するための変数  0: x面, 1: y面
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
			if (game->map[map_y][map_x] > '0')
				hit = 1;
		}

		// 壁までの光線の距離を計算する
		if (side == 0)
			perp_wall_dist = (map_x - game->player.pos.x + (1 - step_x) / 2) / ray_dir.x;
		else
			perp_wall_dist = (map_y - game->player.pos.y + (1 - step_y) / 2) / ray_dir.y;

		// 壁の当たった方角によってテクスチャを変更する
		t_img *tex;
		if (side == 0){
		  if (step_x > 0)
			tex = &(game->tex_n);
		  else
			tex = &(game->tex_s);
		}else{
		  if (step_y > 0)
			tex = &(game->tex_w);
		  else
			tex = &(game->tex_e);
		}

		// スクリーンに描画する必要のある縦線の長さを求める
		int line_height = (int)(wall_height_base / perp_wall_dist);
		// 実際に描画すべき場所の開始位置と終了位置を計算
		int draw_start = -line_height / 2 + game->screen_height / 2;
		if (draw_start < 0)
			draw_start = 0;
		int draw_end = line_height / 2 + game->screen_height / 2;
		if (draw_end >= game->screen_height)
			draw_end = game->screen_height - 1;

		/* 当たった壁上の正確なx座標を求める */
		// 正確なx座標 (整数型ではない)
		double wall_x;
		if (side == 0)
		  wall_x = game->player.pos.y + perp_wall_dist * ray_dir.y;
		else
		  wall_x = game->player.pos.x + perp_wall_dist * ray_dir.x;
		wall_x -= floor(wall_x);  // 正方形のどの部分にヒットしたのか0.0~1.0で表す

		// テクスチャ上のx座標 (0~TEXTURE_WIDTH)
		int texture_x = (int)(wall_x * game->tex_width);
		if ((side == 0 && ray_dir.x > 0) || (side == 1 && ray_dir.y < 0))
		  texture_x = game->tex_width - texture_x - 1;

		/* 各ピクセルにどのテクスチャのピクセルを描画するか計算する */
		// y方向の1ピクセルごとにテクスチャのy座標が動く量
		double step = 1.0 * game->tex_height / (double)line_height;
		// テクスチャの現在のy座標
		double texture_pos_y = (draw_start - game->screen_height / 2 + line_height / 2) * step;
		for (int y = 0; y < game->screen_height; y++)
		{
			if (y <= game->screen_height / 2)
				my_mlx_pixel_put(game, x, y, 0x87ceeb);  // draw sky
			else
				my_mlx_pixel_put(game, x, y, 0x9d6e5e);  // draw ground
			if (y >= draw_start && y < draw_end)
			{
				// テクスチャの現在のy座標(double型)を整数型に変換する.
				int texture_y = (int)texture_pos_y & (game->tex_height - 1);  //  (TEXTURE_HEIGHT - 1)とのANDによりテクスチャ座標がテクスチャの高さを超えないようにしている.
				texture_pos_y += step;
				uint32_t color = get_color_from_img(*tex, texture_x, texture_y);
				// 正方形のy面にヒットしていた場合はRGBのそれぞれを1/2にすることで暗くする
				if (side == 1)
					color = (color >> 1) & 0x7f7f7f;
				my_mlx_pixel_put(game, x, y, color);
			}
		}

		game->z_buffer[x] = perp_wall_dist;

		/*
		printf("ray_dir\t");
		print_vec2(ray_dir);
		printf("hit at map_x: %d, map_y: %d, side: %d\n", map_x, map_y, side);
		printf("delta_dist_x: %lf, delta_dist_y: %lf\n", delta_dist_x, delta_dist_y);
		printf("side_dist_x: %lf, side_dist_y: %lf\n", side_dist_x, side_dist_y);
		printf("step_x: %d, step_y: %d\n", step_x, step_y);
		printf("perp_wall_dist: %lf\n", perp_wall_dist);
		printf("line_height:    %d\n", line_height);
		printf("draw_start:     %d\n", draw_start);
		printf("draw_end:       %d\n", draw_end);
		printf("v_start:\n\tx: %lf\n\ty: %lf\n", v_start.x, v_start.y);
		printf("v_end:\n\tx: %lf\n\ty: %lf\n", v_end.x, v_end.y);
		*/
	}

	/*  スプライトの描画  */

	// スプライトを遠い順にソートする(遠いスプライトから描画するから)
	sort_sprites(game);

	// スプライトのソートが完了したら遠いスプライトから描画していく
	for (int i = 0; i < game->sprite_num; i++){
		printf("---------sprite%d---------\n", i);
		t_vec2 sprite;
		// スプライトの位置をカメラからの相対位置にする
		// 透視投影変換における原点調整
		sprite.x = game->sprites[i].x - game->player.pos.x;
		sprite.y = game->sprites[i].y - game->player.pos.y;

		// カメラ行列の逆行列を掛けてスクリーン上の座標を算出する
		double inv_det = 1.0 / (game->player.plane.x * game->player.dir.y - game->player.dir.x * game->player.plane.y);
		double transform_x = inv_det * (game->player.dir.y * sprite.x - game->player.dir.x * sprite.y);       // カメラ平面(中心を0とする)とスプライトのx座標を比較した時の差
		double transform_y = inv_det * (-game->player.plane.y * sprite.x + game->player.plane.x * sprite.y);  // スプライトまでの深度となる
		printf("sprite_x: %lf\nsprite_y: %lf\n", sprite.x, sprite.y);
		printf("transform_x: %lf\ntransform_y: %lf\n", transform_x, transform_y);

		// スクリーン上でのスプライトの座標
		/* (1.0 + transform_x / transform_y) の説明
		 * この式はによって算出される値は,
		 * スプライトの中心が画面左端にある時は0,
		 * スプライトの中心が画面中央にある時は1,
		 * スプライトの中心が画面左端にある時は2,
		 * となる.
		 * これに画面サイズの半分を掛けることで画面上でのスプライトのx座標がわかる
		 */
		int sprite_screen_x = (int)((game->screen_width / 2) * (1.0 + transform_x / transform_y));
		printf("(1.0 + transform_x / transform_y): %lf\n", (1.0 + transform_x / transform_y));
		printf("((game->screen_width / 2) * (1.0 + transform_x / transform_y)): %d\n", sprite_screen_x);

		// スクリーン上でのスプライトの高さ
		int sprite_height_screen = ABS((int)(game->screen_height / transform_y));

		// スプライト描画の一番下と一番上を計算する
		int draw_start_y = -sprite_height_screen / 2 + game->screen_height / 2;
		if (draw_start_y < 0) draw_start_y = 0;
		int draw_end_y = sprite_height_screen / 2 + game->screen_height / 2;
		if (draw_end_y >= game->screen_height) draw_end_y = game->screen_height - 1;

		// スプライトの横幅を計算する
		int sprite_width_screen = ABS((int)(game->screen_height / transform_y));
		int draw_start_x = -sprite_width_screen / 2 + sprite_screen_x;
		if (draw_start_x < 0) draw_start_x = 0;
		int draw_end_x = sprite_width_screen / 2 + sprite_screen_x;
		if (draw_end_x >= game->screen_width) draw_end_x = game->screen_width - 1;

		// スプライトの各縦線について描画
		for (int stripe = draw_start_x; stripe < draw_end_x; stripe++){
			// テクスチャのx座標
			// (stripe - (-sprite_width_screen / 2 + sprite_screen_x)) で現在何ピクセル目かを取得
			int tex_x = (int)((stripe - (-sprite_width_screen / 2 + sprite_screen_x)) * game->sprite_width / sprite_width_screen);
			/* 以下の条件の時描画を行う
			 * 1. カメラの平面の前にいるか　(カメラ平面の後ろにあるスプライトは描画しない)
			 * 2. スクリーン上にある (left)
			 * 3. スクリーン上にある (right)
			 * 4. zBufferに記録された壁までの距離より近い
			 */
			if (transform_y > 0 && stripe >= 0 && stripe < game->screen_width && transform_y < game->z_buffer[stripe]){
				for (int y = draw_start_y; y < draw_end_y; y++){
					int tex_y = (int)((y - (-sprite_height_screen / 2 + game->screen_height / 2)) * game->tex_height / sprite_height_screen);
					uint32_t color = get_color_from_img(game->tex_sprite, tex_x, tex_y);
					my_mlx_pixel_put(game, stripe, y, color);
					if (tex_x == (game->sprite_width) / 2)
						my_mlx_pixel_put(game, stripe, y, 0xff0000);
				}
			}
		}
	}
}


int		main_loop(t_game *game)
{
	clear_img(game);
	lodev_loop(game);
	// draw_wall(game);
	/*
	draw_player(game);
	print_game(game);
	*/
	move_player(game);
	print_game(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int	load_cubfile(char *filepath)
{
	int		fd;
	char	*line;
	int		status;
	char	**param;

	if ((fd = open(filepath, O_RDONLY)) == -1)
		return (-1);
	while ((status = get_next_line(fd, &line)) == 1)
	{
		param = ft_split(line, ' ');
		free(line);

		if (ft_strncmp(param[0], "R", 1) == 0){

		}else if (ft_strncmp(param[0], "NO", 2) == 0){

		}else if (ft_strncmp(param[0], "SO", 2) == 0){

		}else if (ft_strncmp(param[0], "WE", 2) == 0){

		}else if (ft_strncmp(param[0], "EA", 2) == 0){

		}else if (ft_strncmp(param[0], "S", 1) == 0){

		}else if (ft_strncmp(param[0], "F", 1) == 0){

		}else if (ft_strncmp(param[0], "C", 1) == 0){

		}else{
			// マップの読み込み
		}
	}
	return (status);
}

int main(int argc, char **argv){
	t_game	game;
	/*
	if ((load_cubfile(argv[1])) == -1){
		printf("Error is occured when load cub file\n");
		exit(1);
	}
	*/
	initialize_game(&game);

	mlx_hook(game.win, KeyPress, KeyPressMask, key_press_hook, &game);
	mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release_hook, &game);
	mlx_loop_hook(game.mlx, &main_loop, &game);
    mlx_loop(game.mlx);
}
