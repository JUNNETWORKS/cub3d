/* https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html */
#include "./minilibx-linux/mlx.h"
#include <stdio.h>

// mlxのポインタやウィンドウのポインタを保持
typedef struct  s_vars {
    void        *mlx;
    void        *win;
}               t_vars;

// 描画するためのイメージ情報を保持
typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

// mlx_pixel_put() はレンダリングを待たずに1ピクセルずつ描画するので激遅, なので同じ動きをする関数を自作する
void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

	// line_lengthは実際のウィンドウの横幅と違うので計算する必要がある
    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

// キーボード入力のフック
int 			key_hook(int keycode, t_vars *vars)
{
	printf("Pressed Key Code: %d\n", keycode);
}

// マウスのフック (man mlx_loop)
int 			mouse_hook(int button, int x, int y, t_vars *vars)
{
	printf("Mouse Button: %d\n", button);
	printf("x: %d, y: %d\n", x, y);
}


int     main(void)
{
	t_vars	vars;
    t_data  img;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, 1920, 1080, "Hello world!");
	mlx_key_hook(vars.win, key_hook, &vars);
	mlx_mouse_hook(vars.win, mouse_hook, &vars);
    img.img = mlx_new_image(vars.mlx, 1920, 1080);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);
    my_mlx_pixel_put(&img, 5, 5, 0x00FF0000);  // (5,5)に赤(0, 255, 0, 0)を描画する

	for (int x = 0; x < 1920; x++){
	  for (int y = 0; y < 1080; y++){
		my_mlx_pixel_put(&img, x, y, 0xACDBDA);
	  }
	}

	for (int i = 0; i < 256; i++){
	  for (int j = 0; j< 256; j++){
		my_mlx_pixel_put(&img, i, j, i << 16 | j << 8);
	  }
	}

	// 四角形の描画
	for (int i = 100; i < 200; i++)
	  my_mlx_pixel_put(&img, i, 100, 0x00FFFFFF);
	for (int i = 100; i < 200; i++){
	  my_mlx_pixel_put(&img, 100, i, 0x00FFFFFF);
	  my_mlx_pixel_put(&img, 199, i, 0x00FFFFFF);
	}
	for (int i = 100; i < 200; i++)
	  my_mlx_pixel_put(&img, i, 199, 0x00FFFFFF);

    mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
    mlx_loop(vars.mlx);
}
