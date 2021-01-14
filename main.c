/* https://harm-smits.github.io/42docs/libs/minilibx/getting_started.html */
#include "./minilibx-linux/mlx.h"

typedef struct  s_data {
    void        *img;
    char        *addr;
    int         bits_per_pixel;
    int         line_length;
    int         endian;
}               t_data;

// mlx_pixel_put() は1ピクセルずつ描画するので激遅なので, 同じ動きをする関数を自作する
void            my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char    *dst;

	// line_lengthは実際のウィンドウの横幅と違うので計算する必要がある
    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int     main(void)
{
    void    *mlx;
	t_data  img;

    mlx = mlx_init();
	img.img = mlx_new_image(mlx, 1920, 1080);

	/*
    ** After creating an image, we can call `mlx_get_data_addr`, we pass
    ** `bits_per_pixel`, `line_length`, and `endian` by reference. These will
    ** then be set accordingly for the *current* data address.
    */
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);
}
