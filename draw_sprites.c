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

void	draw_sprite_stripe(t_game *game, t_sprite_vis_info sprite_vis)
{
	int			x;
	int			y;
	int			tex_x;
	int			tex_y;
	uint32_t	color;

	x = sprite_vis.draw_start_x;
	while(x < sprite_vis.draw_end_x)
	{
		// テクスチャのx座標
		// (stripe - (-sprite_width_screen / 2 + screen_x)) で現在何ピクセル目かを取得
		tex_x = (int)((x - (-sprite_vis.width_screen / 2 + sprite_vis.screen_x)) *
			game->tex_sprite.width / sprite_vis.width_screen);
		/* 以下の条件の時描画を行う
		 * 1. カメラの平面の前にいるか　(カメラ平面の後ろにあるスプライトは描画しない)
		 * 2. スクリーン上にある (left)
		 * 3. スクリーン上にある (right)
		 * 4. zBufferに記録された壁までの距離より近い
		 */
		if (sprite_vis.transform_y > 0 &&
			x >= 0 && x < game->screen_width &&
			sprite_vis.transform_y < game->z_buffer[x]){
			y = sprite_vis.draw_start_y;
			while(y < sprite_vis.draw_end_y)
			{
				tex_y = (int)(
					(y - (-sprite_vis.height_screen / 2 + game->screen_height / 2)) *
					game->tex_sprite.height / sprite_vis.height_screen);
				color = get_color_from_img(game->tex_sprite, tex_x, tex_y);
				if (!(color & 0xff000000))
					my_mlx_pixel_put(&(game->img), x, y, color);
				y++;
			}
		}
		x++;
	}
}

void	draw_sprite(t_game *game, t_vec2 sprite)
{
	// スプライトを描画する時に必要な情報を保持する構造体
	t_sprite_vis_info	sprite_vis;

	/* ============== ワールド座標からカメラ座標への変換 ============== */
	calc_transform(game, sprite, &sprite_vis.transform_x, &sprite_vis.transform_y);
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
	sprite_vis.screen_x = (int)((game->screen_width / 2) * (1.0 + sprite_vis.transform_x / sprite_vis.transform_y));
	printf("(1.0 + transform_x / transform_y): %lf\n", (1.0 + sprite_vis.transform_x / sprite_vis.transform_y));
	printf("((game->screen_width / 2) * (1.0 + transform_x / transform_y)): %d\n", sprite_vis.screen_x);
	// スクリーン上でのスプライトの高さ
	sprite_vis.height_screen = ABS((int)(game->height_base / sprite_vis.transform_y));
	// スプライト描画の一番下と一番上を計算する
	sprite_vis.draw_start_y = -sprite_vis.height_screen / 2 + game->screen_height / 2;
	if (sprite_vis.draw_start_y < 0)
		sprite_vis.draw_start_y = 0;
	sprite_vis.draw_end_y = sprite_vis.height_screen / 2 + game->screen_height / 2;
	if (sprite_vis.draw_end_y >= game->screen_height)
		sprite_vis.draw_end_y = (sprite_vis.draw_end_y >= game->screen_height) ? game->screen_height - 1 : sprite_vis.draw_end_y;
	// スプライトの横幅を計算する
	sprite_vis.width_screen = ABS((int)(game->height_base / sprite_vis.transform_y));
	sprite_vis.draw_start_x = -sprite_vis.width_screen / 2 + sprite_vis.screen_x;
	if (sprite_vis.draw_start_x < 0)
		sprite_vis.draw_start_x = 0;
	sprite_vis.draw_end_x = sprite_vis.width_screen / 2 + sprite_vis.screen_x;
	if (sprite_vis.draw_end_x >= game->screen_width)
		sprite_vis.draw_end_x = game->screen_width - 1;

	printf("sprite:\n\twidth: %d\n\theight: %d\n", sprite_vis.width_screen, sprite_vis.height_screen);

	// スプライトの各縦線について描画
	printf("transform_x: %lf, transform_y: %lf\n", sprite_vis.transform_x, sprite_vis.transform_y);
	printf("screen_x: %d, height_screen: %d, width_screen: %d\n", sprite_vis.screen_x, sprite_vis.height_screen, sprite_vis.width_screen);
	printf("draw_start_y: %d, draw_end_y: %d\n", sprite_vis.draw_start_y, sprite_vis.draw_end_y);
	printf("draw_start_x: %d, draw_end_x: %d\n", sprite_vis.draw_start_x, sprite_vis.draw_end_x);
	draw_sprite_stripe(game, sprite_vis);
}

void	draw_sprites(t_game *game)
{
	// planeベクトルの大きさを計算
	double	plane_length;
	int		sprite_idx;

	// スプライトを遠い順にソートする(遠いスプライトから描画するから)
	sort_sprites(game);

	// スプライトのソートが完了したら遠いスプライトから描画していく
	sprite_idx = 0;
	while (sprite_idx < game->sprite_num)
	{
		printf("---------sprite%d---------\n", sprite_idx);
		draw_sprite(game, game->sprites[sprite_idx++]);
	}
}
