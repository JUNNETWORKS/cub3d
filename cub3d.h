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

// display definitions
# define BLOCK_SIZE 64

// Game Settings
# define PLAYER_MOVE_PX (0.025)
# define PLAYER_ROTATE_RAD (M_PI / 300)

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
typedef struct  s_game {
	void		*mlx;
	void		*win;
	t_img		img;
	t_img		img_for_sprite;
	int			screen_width;
	int			screen_height;
	t_player	player;
	char		**map;
	t_img		tex_n; // 北
	t_img		tex_s; // 南
	t_img		tex_w; // 西
	t_img		tex_e; // 東
	int			tex_width;
	int			tex_height;
	uint32_t	sky_color;
	uint32_t	ground_color;

	int			sprite_num;  // スプライトの数
	t_vec2		*sprites;    // スプライトの座標
	int			sprite_width;  // スプライトの描画する時の幅
	int			sprite_height; // スプライトの描画する時の高さ
	t_img		tex_sprite;  // スプライトのテクスチャ
	double		*z_buffer;  // z_buffer[screenWidth]  // スクリーンのxにおける壁までの距離
}               t_game;

// MLX Utils
void			my_mlx_pixel_put(t_img *img, int x, int y, int color);
uint32_t		get_color_from_img(t_img img, int x, int y);
void			load_image(t_game *game, t_img *img, char *filepath);
// Game functions
void			initialize_game(t_game *game);
// Hooks
int 			key_press_hook(int keycode, t_game *game);
int 			key_release_hook(int keycode, t_game *game);
// Drawers
void			clear_img(t_img *img);
void			draw_block(t_img *img, int x, int y, int color);
void			draw_2vec2(t_img *img, t_vec2 v1, t_vec2 v2, int color);
void			draw_line_angle_length(t_img *img, t_vec2 position, double angle, int length, int color);
// Vector
double			vec2_length(t_vec2 vec);         // ベクトルの長さを求める
void			vec2_add(t_vec2 *a, t_vec2 b);   // ベクトルの足し算
void			vec2_mult(t_vec2 *vec, int num);  // ベクトルを定数倍する
void			vec2_rotate(t_vec2 *vec, double rad);  // 回転行列を掛ける
// Player
void			move_player(t_game *game);
void			initialize_player(t_player *player, double x, double y, char direction);
// Utils
double			deg2rad(int x);
int				rad2deg(double x);
uint32_t		rgb2hex(char r, char g, char b);
// Debug
void			print_game(t_game *game);
void			print_vec2(t_vec2 vec);

#endif
