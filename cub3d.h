/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/01 00:29:22 by jtanaka           #+#    #+#             */
/*   Updated: 2021/03/02 04:14:07 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define BLOCK_SIZE 10
# define PLAYER_MOVE_PX (0.033)
# define PLAYER_ROTATE_RAD (M_PI / 300)
# define MAX_MAP_WIDTH 200
# define MAX_MAP_HEIGHT 200
# define PLAYER_INIT_POS_X -1
# define PLAYER_INIT_POS_Y -1

# define KEY_Q 113
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_R_ARROW 65363
# define KEY_L_ARROW 65361

# define ABS(x) ((x) < 0 ? -1 * (x) : (x))
# define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef struct	s_img {
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			width;
	int			height;
}				t_img;

typedef struct	s_vec2 {
	double		x;
	double		y;
}				t_vec2;

typedef struct	s_ray2 {
	t_vec2		pos;
	t_vec2		way;
}				t_ray2;

typedef struct	s_player {
	t_vec2		pos;
	t_vec2		dir;
	t_vec2		plane;
	int			is_moving;
	int			is_sidling;
	int			is_rotating;
}				t_player;

typedef struct	s_texture {
	t_img		img;
	int			width;
	int			height;
}				t_texture;

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
	t_img		tex_n;
	t_img		tex_s;
	t_img		tex_w;
	t_img		tex_e;
	int			tex_width;
	int			tex_height;
	uint32_t	sky_color;
	uint32_t	ground_color;
	double		height_base;

	int			sprite_num;
	t_vec2		*sprites;
	int			sprite_width;
	int			sprite_height;
	t_img		tex_sprite;
	double		*z_buffer;
	double		*sprite_dists;
}				t_game;

typedef struct	s_sprite_vis_info {
	double		transform_x;
	double		transform_y;
	int			screen_x;
	int			height;
	int			draw_start_y;
	int			draw_end_y;
	int			width;
	int			draw_start_x;
	int			draw_end_x;
}				t_sprite_vis_info;

typedef struct	s_ray {
	double		camera_x;
	t_vec2		dir;
	int			map_x;
	int			map_y;
	double		side_dist_x;
	double		side_dist_y;
	double		perp_wall_dist;
	int			side;
	int			step_x;
	int			step_y;
	double		delta_dist_x;
	double		delta_dist_y;
	t_img		*tex;
}				t_ray;

typedef struct	s_wall_vis_info {
	int			line_height;
	int			draw_start;
	int			draw_end;
	double		wall_x;
	int			texture_x;
	double		step;
	double		texture_pos_y;
	int			texture_y;
}				t_wall_vis_info;

void			my_mlx_pixel_put(t_img *img, int x, int y, int color);
uint32_t		get_color(t_img img, int x, int y);
int				load_image(t_game *game, t_img *img, char *filepath);
int				initialize_game(t_game *game);
int				configure_screen(t_game *game, bool has_window);
int				exit_game(t_game *game);
void			draw_walls(t_game *game);
void			draw_sprites(t_game *game);
void			draw_sprite_stripe(t_game *game, t_sprite_vis_info sprite_vis);
int				load_cubfile(t_game *game, char *filepath);
int				check_map_surrounded(t_game *game);
bool			is_valid_color(int r, int g, int b);
uint32_t		get_color_from_rgbstr(char *rgbstr, uint32_t *color);
bool			has_color_set(t_game *game, char name);
int				set_color(t_game *game, char name, char *rgbstr);
int				load_texture(t_game *game, char *name, char *texture_path);
int				load_map(t_game *game, char *line);
int				get_pos_from_map(t_game *game);
int				set_resolution(t_game *game, char *width_str, char *height_str);
bool			has_config_already_set(t_game *game);
int				key_press_hook(int keycode, t_game *game);
int				key_release_hook(int keycode, t_game *game);
void			clear_img(t_img *img);
void			draw_block(t_img *img, int x, int y, int color);
void			draw_line_angle_length(t_img *img, t_vec2 position,
					double angle, int length, int color);
double			vec2_length(t_vec2 vec);
void			vec2_add(t_vec2 *a, t_vec2 b);
void			vec2_mult(t_vec2 *vec, int num);
void			vec2_rotate(t_vec2 *vec, double rad);
void			update_player(t_game *game);
int				initialize_player(t_player *player,
					double x, double y, char direction);
int				add_sprite(t_game *game, double x, double y);
void			sort_sprites(t_game *game);
int				write_game2bmp(t_game *game, char *filepath);
double			deg2rad(int x);
int				rad2deg(double x);
uint32_t		rgb2hex(int r, int g, int b);
uint32_t		alpha_blend(uint32_t dst, uint32_t src);
void			put_error_msg(char *str);
int				put_and_return_err(char *str);
int				put_and_exit_err(char *str);
bool			str_all_true(char *str, int(*f)(int));
size_t			str_c_count(char *str, char c);
size_t			ptrarr_len(void **ptrarr);
void			free_ptrarr(void **ptrarr);
void			free_ptrarr_and_assign_null(void ***ptrarr);
void			free_and_assign_null(void **p);
void			print_game(t_game *game);
void			print_vec2(t_vec2 vec);

#endif
