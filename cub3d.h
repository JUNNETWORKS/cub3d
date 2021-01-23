#ifndef CUB3D_H
# define CUB3D_H
# include "./minilibx-linux/mlx.h"
# include "./libft/libft.h"
# include <X11/X.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <math.h>

// display definitions
# define WIDTH 1920
# define HEIGHT 1080
# define BLOCK_SIZE 64

// Game Settings
# define PLAYER_MOVE_PX (4)
# define PLAYER_ROTATE_RAD (M_PI / 100)

// Key definitions
# define KEY_q 113
# define KEY_esc 65307
# define KEY_w 119
# define KEY_a 97
# define KEY_s 115
# define KEY_d 100

// utils
# define ABS(x) ((x) < 0 ? -1 * (x) : (x))

// 描画するためのイメージ情報を保持
typedef struct	s_img {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
}				t_img;

// Vector2D
typedef struct	s_vec2 {
	int			x;
	int			y;
} t_vec2;


// Ray 2D
typedef struct	s_ray2 {
	t_vec2		pos;  // レイの始点の位置ベクトル
	t_vec2		way;  // レイの始点から伸びる方向ベクトル
} t_ray2;

// Player
typedef struct	s_player {
	t_vec2		position;  // 現在位置(px)[x, y]
	int			is_moving; // 動くキーが押されているか (W=1, S=-1, None=0)
	double		angle;      // 角度(rad)
	int			is_rotating; // 動くキーが押されているか (D=1, A=-1, None=0)
}				t_player;

// mlxのポインタやウィンドウのポインタを保持
typedef struct  s_game {
	void		*mlx;
	void		*win;
	t_img		img;
	t_player	player;
	char		**map;
}               t_game;

// MLX Utils
void			my_mlx_pixel_put(t_game *game, int x, int y, int color);
// Game functions
void			initialize_game(t_game *game);
// Hooks
int 			key_press_hook(int keycode, t_game *game);
int 			key_release_hook(int keycode, t_game *game);
// Drawers
void	clear_img(t_game *game);
void	draw_block(t_game *game, int x, int y, int color);
void	draw_2vec2(t_game *game, t_vec2 v1, t_vec2 v2, int color);
void	draw_line_angle_length(t_game *game, t_vec2 position, double angle, int length, int color);
// Vector
double	vec2_length(t_vec2 vec);         // ベクトルの長さを求める
void	vec2_add(t_vec2 *a, t_vec2 b);   // ベクトルの足し算
void	vec2_mult(t_vec2 vec, int num);  // ベクトルを定数倍する
// Utils
double	deg2rad(int x);
// Debug
void			print_game(t_game *game);

#endif
