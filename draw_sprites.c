#include "cub3d.h"

void	calc_transform(t_game *game, t_vec2 sprite, double *transform_x, double *transform_y)
{
	// カメラ行列の逆行列
	double inv_det;
	// カメラ平面(中心を0とする)とスプライトのx座標を比較した時の差
	// double transform_x;
	// スプライトまでの深度となる
	// double transform_y;
	// スプライトを描画する中心x座標
	t_vec2 normalized_sprite;
	// スプライトを描画するのに必要な情報を保持する構造体

	// スプライトの位置をカメラからの相対位置にする
	// 透視投影変換における原点調整
	normalized_sprite.x = sprite.x - game->player.pos.x;
	normalized_sprite.y = sprite.y - game->player.pos.y;
	// カメラ行列の逆行列を掛けてスクリーン上の座標を算出する
	inv_det = 1.0 / (game->player.plane.x * game->player.dir.y - game->player.dir.x * game->player.plane.y);
	*transform_x = inv_det * (game->player.dir.y * normalized_sprite.x - game->player.dir.x * normalized_sprite.y);
	*transform_y = inv_det * (-game->player.plane.y * normalized_sprite.x + game->player.plane.x * normalized_sprite.y);
	printf("sprite_x: %lf\nsprite_y: %lf\n", normalized_sprite.x, normalized_sprite.y);
	printf("transform_x: %lf\ntransform_y: %lf\n", *transform_x, *transform_y);
}

void	draw_sprite(t_game *game, t_vec2 sprite, double plane_length, double height_base)
{
	// スプライトを描画する時に必要な情報を保持する構造体
	t_sprite_vis_info sprite_vis_info;
	/* ============== ワールド座標からカメラ座標への変換 ============== */
	calc_transform(game, sprite, &sprite_vis_info.transform_x, &sprite_vis_info.transform_y);
	/* ============== 実際に描画すべき座標の計算 ============== */
	// スクリーン上でのスプライトの座標
	/* (1.0 + transform_x / transform_y) の説明
	 * この式はによって算出される値は,
	 * スプライトの中心が画面左端にある時は0,
	 * スプライトの中心が画面中央にある時は1,
	 * スプライトの中心が画面左端にある時は2,
	 * となる.
	 * これに画面サイズの半分を掛けることで画面上でのスプライトのx座標がわかる
	 */
	/*
	// スクリーン上でのスプライトの座標
	int sprite_screen_x;
	// スクリーン上でのスプライトの高さ
	int sprite_height_screen;
	// スプライト描画の一番上
	int draw_start_y;
	// スプライト描画の一番下
	int draw_end_y;
	// スクリーン上でのスプライトの横幅
	int sprite_width_screen;
	// スプライト描画の一番左
	int draw_start_x;
	// スプライト描画の一番右
	int draw_end_x;

	sprite_screen_x = (int)((game->screen_width / 2) * (1.0 + transform_x / transform_y));
	printf("(1.0 + transform_x / transform_y): %lf\n", (1.0 + transform_x / transform_y));
	printf("((game->screen_width / 2) * (1.0 + transform_x / transform_y)): %d\n", sprite_screen_x);
	// スクリーン上でのスプライトの高さ
	sprite_height_screen = ABS((int)(height_base / transform_y));
	// スプライト描画の一番下と一番上を計算する
	draw_start_y = -sprite_height_screen / 2 + game->screen_height / 2;
	if (draw_start_y < 0)
		draw_start_y = 0;
	draw_end_y = sprite_height_screen / 2 + game->screen_height / 2;
	if (draw_end_y >= game->screen_height)
		draw_end_y = game->screen_height - 1;
	// スプライトの横幅を計算する
	sprite_width_screen = ABS((int)(height_base / transform_y));
	draw_start_x = -sprite_width_screen / 2 + sprite_screen_x;
	if (draw_start_x < 0) draw_start_x = 0;
	draw_end_x = sprite_width_screen / 2 + sprite_screen_x;
	if (draw_end_x >= game->screen_width) draw_end_x = game->screen_width - 1;

	printf("sprite:\n\twidth: %d\n\theight: %d\n", sprite_width_screen, sprite_height_screen);

	// スプライトの各縦線について描画
	for (int stripe = draw_start_x; stripe < draw_end_x; stripe++){
		// テクスチャのx座標
		// (stripe - (-sprite_width_screen / 2 + sprite_screen_x)) で現在何ピクセル目かを取得
		int tex_x = (int)((stripe - (-sprite_width_screen / 2 + sprite_screen_x)) * game->tex_sprite.width / sprite_width_screen);
		/* 以下の条件の時描画を行う
		 * 1. カメラの平面の前にいるか　(カメラ平面の後ろにあるスプライトは描画しない)
		 * 2. スクリーン上にある (left)
		 * 3. スクリーン上にある (right)
		 * 4. zBufferに記録された壁までの距離より近い
		 */
		if (transform_y > 0 && stripe >= 0 && stripe < game->screen_width && transform_y < game->z_buffer[stripe]){
			for (int y = draw_start_y; y < draw_end_y; y++){
				int tex_y = (int)((y - (-sprite_height_screen / 2 + game->screen_height / 2)) * game->tex_sprite.height / sprite_height_screen);
				uint32_t color = get_color_from_img(game->tex_sprite, tex_x, tex_y);
				if (color & 0xff000000)
				  continue;
				my_mlx_pixel_put(&(game->img), stripe, y, color);
				if (tex_x == (game->tex_sprite.width) / 2)
					my_mlx_pixel_put(&(game->img), stripe, y, 0xff0000);
			}
		}
	}
}

void	draw_sprites(t_game *game)
{
	// planeベクトルの大きさを計算
	double	plane_length;
	// 基準となるスプライトの高さ. 視野角に応じて横幅が変わってしまうので, 視野角の逆数を掛けて1に戻す
	double	height_base;
	int		sprite_idx;

	// planeベクトルの大きさを計算
	plane_length = vec2_length(game->player.plane);
	// 基準となる壁の高さ. 視野角に応じて横幅が変わってしまうので, 視野角の逆数を掛けて1に戻す
	height_base = (double)game->screen_width * (1 / (2 * plane_length));

	// スプライトを遠い順にソートする(遠いスプライトから描画するから)
	sort_sprites(game);

	// スプライトのソートが完了したら遠いスプライトから描画していく
	sprite_idx = 0;
	while (sprite_idx < game->sprite_num)
	{
		printf("---------sprite%d---------\n", sprite_idx);
		draw_sprite(game, game->sprites[sprite_idx++], plane_length, height_base);
	}
}
