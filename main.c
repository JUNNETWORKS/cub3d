#include "cub3d.h"

void	lodev_loop(t_game *game)
{
	double	plane_length;
	double	wall_height_base;

	// planeベクトルの大きさを計算
	plane_length = vec2_length(game->player.plane);
	// 基準となる壁の高さ. 視野角に応じて横幅が変わってしまうので, 視野角の逆数を掛けて1に戻す
	wall_height_base = (double)game->screen_width * (1 / (2 * plane_length));
	// スクリーンの全てのxについて計算する
	for (int x = 0; x < game->screen_width; x++)
	{
		// カメラ平面上のx座標 (3D表示時の画面のx座標)  -1.0~1.0
		double	camera_x;
		// 光線ベクトル
		t_vec2 ray_dir;
		// map: 現在対象としているマップ内の正方形を表す
		int map_x;
		int map_y;
		// sideDistは, 光線が開始位置から最初の次の正方形に移動するまでの距離
		double side_dist_x;
		double side_dist_y;
		// 壁に衝突したか
		int hit = 0;
		// perpWallDistは, 当たった壁とカメラ平面ベクトルとの距離を表す
		double perp_wall_dist;
		// 壁のx面かy面どちらに当たったかを判断するための変数  0: x面, 1: y面
		int side;
		// stepはx,yそれぞれ正か負かどちらの方向に進むか記録する (必ず +1 or -1)
		int step_x;
		int step_y;
		// deltaDistは, 光線が今の正方形から次の正方形に行くために移動する距離
		double delta_dist_x;
		double delta_dist_y;

		/* ============== 初期設定 ============== */
		// カメラ平面上のx座標 (3D表示時の画面のx座標)  -1.0~1.0
		camera_x = 2 * x / (double)game->screen_width - 1;
		ray_dir.x = game->player.dir.x + game->player.plane.x * camera_x;
		ray_dir.y = game->player.dir.y + game->player.plane.y * camera_x;
		// map: 現在対象としているマップ内の正方形を表す
		map_x = (int)game->player.pos.x;
		map_y = (int)game->player.pos.y;
		// deltaDistは, 光線が今の正方形から次の正方形に行くために移動する距離
		delta_dist_x = (1 / ray_dir.x) < 0 ? -1 * (1 / ray_dir.x) : (1 / ray_dir.x);
		delta_dist_y = (1 / ray_dir.y) < 0 ? -1 * (1 / ray_dir.y) : (1 / ray_dir.y);
		// 壁に衝突したか
		hit = 0;
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

		/* ============== 壁まで光線を飛ばす ============== */
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
			if (game->map[map_y][map_x] == '1')
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
			tex = &game->tex_w;
		  else
			tex = &game->tex_e;
		}else{
		  if (step_y > 0)
			tex = &game->tex_n;
		  else
			tex = &game->tex_s;
		}

		/* ============== 描画 ============== */
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
		int texture_x = (int)(wall_x * tex->width);
		if ((side == 0 && ray_dir.x < 0) || (side == 1 && ray_dir.y > 0))
		  texture_x = tex->width - texture_x - 1;

		/* 各ピクセルにどのテクスチャのピクセルを描画するか計算する */
		// y方向の1ピクセルごとにテクスチャのy座標が動く量
		double step = 1.0 * tex->height / (double)line_height;
		// テクスチャの現在のy座標
		double texture_pos_y = (draw_start - game->screen_height / 2 + line_height / 2) * step;
		for (int y = 0; y < game->screen_height; y++)
		{
			if (y <= game->screen_height / 2)
				my_mlx_pixel_put(&(game->img), x, y, game->sky_color);  // draw sky
			else
				my_mlx_pixel_put(&(game->img), x, y, game->ground_color);  // draw ground
			if (y >= draw_start && y < draw_end)
			{
				// テクスチャの現在のy座標(double型)を整数型に変換する.
				int texture_y = (int)texture_pos_y & (tex->height - 1);  //  (TEXTURE_HEIGHT - 1)とのANDによりテクスチャ座標がテクスチャの高さを超えないようにしている.
				texture_pos_y += step;
				uint32_t color = get_color_from_img(*tex, texture_x, texture_y);
				// 正方形のy面にヒットしていた場合はRGBのそれぞれを1/2にすることで暗くする
				if (side == 1)
					color = (color >> 1) & 0x7f7f7f;
				my_mlx_pixel_put(&(game->img), x, y, color);
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

	/* ============== スプライトの描画 ============== */
	draw_sprites(game);
}

void	draw_minimap(t_game *game)
{
	// マップの描画
	for (int i = 0; i < game->map_row; i++){
	  for (int j = 0; j < game->map_col; j++){
		// まずマスの端を描画して, 壁ならば中を塗りつぶす
		// for (int x = TILE_SIZE * j; x < TILE_SIZE * (j + 1); x++)
		if (game->map[i][j] == '1')
			draw_block(&game->img, BLOCK_SIZE * j, BLOCK_SIZE * i, 0x000000);
		else if (game->map[i][j] == '2')
			draw_block(&game->img, BLOCK_SIZE * j, BLOCK_SIZE * i, 0x00ff00);
		else
			draw_block(&game->img, BLOCK_SIZE * j, BLOCK_SIZE * i, 0xffffff);
	  }
	}
	// プレイヤーの描画
	for (int x = game->player.pos.x * BLOCK_SIZE - BLOCK_SIZE / 2; x < game->player.pos.x * BLOCK_SIZE + BLOCK_SIZE / 2; x++){
		for (int y = game->player.pos.y * BLOCK_SIZE - BLOCK_SIZE / 2; y < game->player.pos.y * BLOCK_SIZE + BLOCK_SIZE / 2; y++){
			if (x < 0 || x >= game->map_col * BLOCK_SIZE || y < 0 || y >= game->map_row * BLOCK_SIZE)
				continue;
			my_mlx_pixel_put(&game->img, x, y, 0xff0000);
		}
	}
	// visualize what direction player is seeing
	t_vec2 pos;
	pos.x = game->player.pos.x * BLOCK_SIZE;
	pos.y = game->player.pos.y * BLOCK_SIZE;
	t_vec2 dir_line;
	dir_line.x = pos.x + game->player.dir.x * BLOCK_SIZE;
	dir_line.y = pos.y + game->player.dir.y * BLOCK_SIZE;
	draw_2vec2(&game->img, pos, dir_line, 0x0000ff, 3);
	// visualize plane
	t_vec2 plane_left;
	plane_left.x = dir_line.x - game->player.plane.x * BLOCK_SIZE;
	plane_left.y = dir_line.y - game->player.plane.y * BLOCK_SIZE;
	t_vec2 plane_right;
	plane_right.x = dir_line.x + game->player.plane.x * BLOCK_SIZE;
	plane_right.y = dir_line.y + game->player.plane.y * BLOCK_SIZE;
	draw_2vec2(&game->img, pos, plane_left, 0x00ff00, 3);
	draw_2vec2(&game->img, pos, plane_right, 0x00ff00, 3);
	draw_2vec2(&game->img, plane_left, plane_right, 0x00ff00, 3);
	// lengthen left and right plane vector
	double lengthen_power = BLOCK_SIZE * 200;
	t_vec2 long_dir_line;
	long_dir_line.x = pos.x + game->player.dir.x * lengthen_power;
	long_dir_line.y = pos.y + game->player.dir.y * lengthen_power;
	t_vec2 long_plane_left;
	long_plane_left.x = long_dir_line.x - game->player.plane.x * lengthen_power;
	long_plane_left.y = long_dir_line.y - game->player.plane.y * lengthen_power;
	t_vec2 long_plane_right;
	long_plane_right.x = long_dir_line.x + game->player.plane.x * lengthen_power;
	long_plane_right.y = long_dir_line.y + game->player.plane.y * lengthen_power;
	draw_2vec2(&game->img, pos, long_plane_left, 0xffff00, 3);
	draw_2vec2(&game->img, pos, long_plane_right, 0xffff00, 3);
}


int		main_loop(t_game *game)
{
	clear_img(&game->img);
	// lodev_loop(game);
	draw_walls(game);
	draw_sprites(game);
	draw_minimap(game);
	update_player(game);
	print_game(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int main(int argc, char **argv){
	if (argc < 2 || argc > 3){  // ./cub3D *.cub --save
		put_error_msg("args count is incorrect!");
		exit(EXIT_FAILURE);
	}
	t_game	game;
	if ((initialize_game(&game)) == ERROR || (load_cubfile(&game, argv[1])) == ERROR){
		put_error_msg("Error is occured when load cub file");
		exit(EXIT_FAILURE);
	}
	if (argc == 3){
		if (ft_strncmp(argv[2], "--save", ft_strlen("--save") + 1)){
			put_error_msg("argv is not \"--save\"\n");
			exit(EXIT_FAILURE);
		}
		configure_screen(&game, false);
		lodev_loop(&game);
		write_game2bmp(&game, "output.bmp");
		exit(EXIT_SUCCESS);
	}
	configure_screen(&game, true);
	mlx_hook(game.win, KeyPress, KeyPressMask, key_press_hook, &game);
	mlx_hook(game.win, KeyRelease, KeyReleaseMask, key_release_hook, &game);
	mlx_loop_hook(game.mlx, &main_loop, &game);
	mlx_loop(game.mlx);
}
