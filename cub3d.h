#ifndef CUB3D_H
# define CUB3D_H
# include "./minilibx-linux/mlx.h"
# include "./libft/libft.h"
# include <X11/X.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <errno.h>
# include <string.h>

// display definitions
# define BLOCK_SIZE 10

// Game Settings
# define PLAYER_MOVE_PX (0.033)
# define PLAYER_ROTATE_RAD (M_PI / 300)
# define MAX_MAP_WIDTH 200
# define MAX_MAP_HEIGHT 200
# define PLAYER_INIT_POS_X -1
# define PLAYER_INIT_POS_Y -1

// Key definitions
# define KEY_q 113
# define KEY_esc 65307
# define KEY_w 119
# define KEY_a 97
# define KEY_s 115
# define KEY_d 100
# define KEY_rallow 65363
# define KEY_lallow 65361

// utils
# define ABS(x) ((x) < 0 ? -1 * (x) : (x))
# define MIN(x, y) ((x) < (y) ? (x) : (y))

// 描画するためのイメージ情報を保持
typedef struct	s_img {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}				t_img;

// Vector2D
typedef struct	s_vec2 {
	double		x;
	double		y;
} t_vec2;


// Ray 2D
typedef struct	s_ray2 {
	t_vec2		pos;  // レイの始点の位置ベクトル
	t_vec2		way;  // レイの始点から伸びる方向ベクトル
} t_ray2;

// Player
typedef struct	s_player {
	t_vec2		pos;  // 現在位置(px)[x, y]
	t_vec2		dir;       // 現在向いている方向のベクトル
	t_vec2		plane;     // 2Dレイキャスティング用のカメラ平面
	int			is_moving; // 動くキーが押されているか (W=1, S=-1, None=0)
	int			is_sidling;  // 動くキーが押されているか (D=1, A=-1, None=0)
	int			is_rotating; // 動くキーが押されているか (左矢印=1, 右矢印=-1, None=0)
}				t_player;

// texture
typedef struct	s_texture {
	t_img		img;
	int			width;
	int			height;
}				t_texture;

enum e_nswe {
  NORTH,
  SOUTH,
  WEST,
  EAST,
};

// mlxのポインタやウィンドウのポインタを保持
typedef struct	s_game {
	void		*mlx;
	void		*win;
	t_img		img;
	int			screen_width;
	int			screen_height;
	t_player	player;
	char		**map;
	int			map_row;
	int			map_col;
	t_img		tex_n; // 北
	t_img		tex_s; // 南
	t_img		tex_w; // 西
	t_img		tex_e; // 東
	int			tex_width;
	int			tex_height;
	uint32_t	sky_color;
	uint32_t	ground_color;
	double		height_base;

	int			sprite_num;  // スプライトの数
	t_vec2		*sprites;    // スプライトの座標
	int			sprite_width;  // スプライトの描画する時の幅
	int			sprite_height; // スプライトの描画する時の高さ
	t_img		tex_sprite;  // スプライトのテクスチャ
	double		*z_buffer;  // z_buffer[screenWidth]  // スクリーンのxにおける壁までの距離
}				t_game;

// スプライトを描画する時に必要な情報を保持する構造体
typedef struct	s_sprite_vis_info {
	// カメラ平面(中心を0とする)とスプライトのx座標を比較した時の差
	double		transform_x;
	// スプライトまでの深度となる
	double		transform_y;
	// スクリーン上でのスプライトの座標
	int			screen_x;
	// スクリーン上でのスプライトの高さ
	int			height;
	// スプライト描画の一番上
	int			draw_start_y;
	// スプライト描画の一番下
	int			draw_end_y;
	// スクリーン上でのスプライトの横幅
	int			width;
	// スプライト描画の一番左
	int			draw_start_x;
	// スプライト描画の一番右
	int			draw_end_x;
}				t_sprite_vis_info;

// スプライトの情報を保持する構造体
typedef struct	s_ray {
	// カメラ平面上のx座標 (3D表示時の画面のx座標)  -1.0~1.0
	double		camera_x;
	// 光線ベクトル
	t_vec2		dir;
	// map: 現在対象としているマップ内の正方形を表す
	int			map_x;
	int			map_y;
	// sideDistは, 光線が開始位置から最初の次の正方形に移動するまでの距離
	double		side_dist_x;
	double		side_dist_y;
	// perpWallDistは, 当たった壁とカメラ平面ベクトルとの距離を表す (perpはperpendicular(垂直)の略)
	double		perp_wall_dist;
	// 壁のx面かy面どちらに当たったかを判断するための変数  0: x面, 1: y面
	int			side;
	// stepはx,yそれぞれ正か負かどちらの方向に進むか記録する (必ず +1 or -1)
	int			step_x;
	int			step_y;
	// deltaDistは, 光線が今の正方形から次の正方形に行くために移動する距離
	double		delta_dist_x;
	double		delta_dist_y;
	// texは当たったテクスチャ
	t_img		*tex;
}				t_ray;

// 壁を描画するのに必要な情報を保持する構造体
typedef struct	s_wall_vis_info {
	// スクリーンに描画する必要のある縦線の長さを求める
	int			line_height;
	// 実際に描画すべき場所の開始位置
	int			draw_start;
	// 実際に描画すべき場所の位置
	int			draw_end;
	// 正確なx座標 (整数型ではない)
	double		wall_x;
	// テクスチャ上のx座標 (0~TEXTURE_WIDTH)
	int			texture_x;
	// y方向の1ピクセルごとにテクスチャのy座標が動く量
	double		step;
	// テクスチャの現在のy座標
	double		texture_pos_y;
	// テクスチャの現在のy座標(double型)を整数型に変換する.
	int			texture_y;
}				t_wall_vis_info;

// MLX Utils
void			my_mlx_pixel_put(t_img *img, int x, int y, int color);
uint32_t		get_color(t_img img, int x, int y);
int				load_image(t_game *game, t_img *img, char *filepath);
// Game functions
int				initialize_game(t_game *game);
void			configure_screen(t_game *game, bool has_window);
// Draw screen
void			draw_walls(t_game *game);
void			draw_sprites(t_game *game);
void			draw_sprite_stripe(t_game *game, t_sprite_vis_info sprite_vis);
// load cubfile
int				load_cubfile(t_game *game, char *filepath);
int				check_map_surrounded(t_game *game);
bool			is_valid_color(int r, int g, int b);
uint32_t		get_color_from_rgbstr(char *rgbstr, uint32_t *color);
bool			has_color_set(t_game *game, char name);
int				set_color(t_game *game, char name, char *rgbstr);
int				load_texture(t_game *game, char *name, char *texture_path);
int				set_resolution(t_game *game, char *width_str, char *height_str);
// Hooks
int 			key_press_hook(int keycode, t_game *game);
int 			key_release_hook(int keycode, t_game *game);
// Drawers
void			clear_img(t_img *img);
void			draw_block(t_img *img, int x, int y, int color);
void			draw_line_angle_length(t_img *img, t_vec2 position, double angle, int length, int color);
// Vector
double			vec2_length(t_vec2 vec);         // ベクトルの長さを求める
void			vec2_add(t_vec2 *a, t_vec2 b);   // ベクトルの足し算
void			vec2_mult(t_vec2 *vec, int num);  // ベクトルを定数倍する
void			vec2_rotate(t_vec2 *vec, double rad);  // 回転行列を掛ける
// Player
void			update_player(t_game *game);
void			initialize_player(t_player *player, double x, double y, char direction);
// Sprite
int				add_sprite(t_game *game, double x, double y);
void			sort_sprites(t_game *game);
// BMP
int				write_game2bmp(t_game *game, char *filepath);
// Utils
double			deg2rad(int x);
int				rad2deg(double x);
uint32_t		rgb2hex(int r, int g, int b);
uint32_t		alpha_blend(uint32_t dst, uint32_t src);
void			put_error_msg(char *str);
int				put_and_return_err(char *str);
bool			str_all_true(char *str, int(*f)(int));
size_t			str_c_count(char *str, char c);
size_t			ptrarr_len(void **ptrarr);
void			free_ptrarr(void **ptrarr);
void			free_ptrarr_and_assign_null(void ***ptrarr);
void			free_and_assign_null(void **p);
// Debug
void			print_game(t_game *game);
void			print_vec2(t_vec2 vec);

#endif
