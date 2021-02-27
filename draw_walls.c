#include "cub3d.h"

void	initialize_ray(t_game *game, t_ray *ray, int x)
{
	// カメラ平面上のx座標 (3D表示時の画面のx座標)  -1.0~1.0
	ray->camera_x = 2 * x / (double)game->screen_width - 1;
	ray->dir.x = game->player.dir.x + game->player.plane.x * ray->camera_x;
	ray->dir.y = game->player.dir.y + game->player.plane.y * ray->camera_x;
	// map: 現在対象としているマップ内の正方形を表す
	ray->map_x = (int)game->player.pos.x;
	ray->map_y = (int)game->player.pos.y;
	// deltaDistは, 光線が今の正方形から次の正方形に行くために移動する距離
	ray->delta_dist_x = (1 / ray->dir.x) < 0 ? -1 * (1 / ray->dir.x) : (1 / ray->dir.x);
	ray->delta_dist_y = (1 / ray->dir.y) < 0 ? -1 * (1 / ray->dir.y) : (1 / ray->dir.y);
	// 壁に衝突したか
	ray->hit = 0;
	// stepとsideDistを求める
	if (ray->dir.x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.pos.x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player.pos.x) * ray->delta_dist_x;
	}
	if (ray->dir.y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.pos.y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player.pos.y) * ray->delta_dist_y;
	}
}

void	simulate_ray(t_game *game, t_ray *ray)
{
	/* ============== 壁まで光線を飛ばす ============== */
	// 光線が壁にぶつかるまで光線を進める
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		// 光線が壁にぶつかったか確認する
		if (game->map[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}

	// 壁までの光線の距離を計算する
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player.pos.x + (1 - ray->step_x) / 2) / ray->dir.x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player.pos.y + (1 - ray->step_y) / 2) / ray->dir.y;

	// 壁の当たった方角によってテクスチャを変更する
	if (ray->side == 0)
		ray->tex = (ray->step_x > 0) ? &game->tex_w : &game->tex_e;
	else
		ray->tex = (ray->step_y > 0) ? &game->tex_n : &game->tex_s;
}

void	draw_stripe(t_game *game, int x, double plane_length, double height_base)
{
	t_ray	ray;

	/* ============== 初期設定 ============== */
	initialize_ray(game, &ray, x);
	/* ============== 壁まで光線を飛ばす ============== */
	simulate_ray(game, &ray);
	/* ============== 描画 ============== */
	// スクリーンに描画する必要のある縦線の長さを求める
	int line_height;
	// 実際に描画すべき場所の開始位置
	int draw_start;
	// 実際に描画すべき場所の位置
	int draw_end;
	// 正確なx座標 (整数型ではない)
	double wall_x;
	// テクスチャ上のx座標 (0~TEXTURE_WIDTH)
	int texture_x;
	// y方向の1ピクセルごとにテクスチャのy座標が動く量
	double step;
	// テクスチャの現在のy座標
	double texture_pos_y;
	int y;
	// テクスチャの現在のy座標(double型)を整数型に変換する.
	int texture_y;
	uint32_t color;

	// スクリーンに描画する必要のある縦線の長さを求める
	line_height = (int)(height_base / ray.perp_wall_dist);
	// 実際に描画すべき場所の開始位置と終了位置を計算
	draw_start = -line_height / 2 + game->screen_height / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + game->screen_height / 2;
	if (draw_end >= game->screen_height)
		draw_end = game->screen_height - 1;

	/* 当たった壁上の正確なx座標を求める */
	if (ray.side == 0)
	  wall_x = game->player.pos.y + ray.perp_wall_dist * ray.dir.y;
	else
	  wall_x = game->player.pos.x + ray.perp_wall_dist * ray.dir.x;
	wall_x -= floor(wall_x);  // 正方形のどの部分にヒットしたのか0.0~1.0で表す

	// テクスチャ上のx座標 (0~TEXTURE_WIDTH)
	texture_x = (int)(wall_x * ray.tex->width);
	if ((ray.side == 0 && ray.dir.x < 0) || (ray.side == 1 && ray.dir.y > 0))
	  texture_x = ray.tex->width - texture_x - 1;

	/* 各ピクセルにどのテクスチャのピクセルを描画するか計算する */
	// y方向の1ピクセルごとにテクスチャのy座標が動く量
	step = 1.0 * ray.tex->height / (double)line_height;
	// テクスチャの現在のy座標
	texture_pos_y = (draw_start - game->screen_height / 2 + line_height / 2) * step;
	y = 0;
	while (y < game->screen_height)
	{
		if (y <= game->screen_height / 2)
			my_mlx_pixel_put(&(game->img), x, y, game->sky_color);  // draw sky
		else
			my_mlx_pixel_put(&(game->img), x, y, game->ground_color);  // draw ground
		if (y >= draw_start && y < draw_end)
		{
			// テクスチャの現在のy座標(double型)を整数型に変換する.
			texture_y = (int)texture_pos_y & (ray.tex->height - 1);  //  (TEXTURE_HEIGHT - 1)とのANDによりテクスチャ座標がテクスチャの高さを超えないようにしている.
			texture_pos_y += step;
			color = get_color_from_img(*ray.tex, texture_x, texture_y);
			// 正方形のy面にヒットしていた場合はRGBのそれぞれを1/2にすることで暗くする
			if (ray.side == 1)
				color = (color >> 1) & 0x7f7f7f;
			my_mlx_pixel_put(&(game->img), x, y, color);
		}
		y++;
	}
	game->z_buffer[x] = ray.perp_wall_dist;
	/*
	printf("ray.dir\t");
	print_vec2(ray.dir);
	printf("hit at map_x: %d, map_y: %d, side: %d\n", ray.map_x, ray.map_y, ray.side);
	printf("delta_dist_x: %lf, delta_dist_y: %lf\n", ray.delta_dist_x, ray.delta_dist_y);
	printf("side_dist_x: %lf, side_dist_y: %lf\n", ray.side_dist_x, ray.side_dist_y);
	printf("step_x: %d, step_y: %d\n", ray.step_x, ray.step_y);
	printf("perp_wall_dist: %lf\n", ray.perp_wall_dist);
	printf("line_height:    %d\n", line_height);
	printf("draw_start:     %d\n", draw_start);
	printf("draw_end:       %d\n", draw_end);
	printf("v_start:\n\tx: %lf\n\ty: %lf\n", v_start.x, v_start.y);
	printf("v_end:\n\tx: %lf\n\ty: %lf\n", v_end.x, v_end.y);
	*/
}

void	draw_walls(t_game *game)
{
	int		x;
	double	plane_length;
	double	wall_height_base;

	// planeベクトルの大きさを計算
	plane_length = vec2_length(game->player.plane);
	// 基準となる壁の高さ. 視野角に応じて横幅が変わってしまうので, 視野角の逆数を掛けて1に戻す
	wall_height_base = (double)game->screen_width * (1 / (2 * plane_length));
	// スクリーンの全てのxについて計算する
	x = 0;
	while(x < game->screen_width)
		draw_stripe(game, x++, plane_length, wall_height_base);
}

