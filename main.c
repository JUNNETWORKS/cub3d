#include "cub3d.h"

char *MAP[] = {
  "1111111111",
  "1000000001",
  "1000000001",
  "1000000001",
  "1111111111",
};

int WIDTH = 1920;
int HEIGHT = 1080;

int main(){
	t_vars	vars;
    t_data  img;

    vars.mlx = mlx_init();
    vars.win = mlx_new_window(vars.mlx, WIDTH, HEIGHT, "Hello world!");
    img.img = mlx_new_image(vars.mlx, WIDTH, HEIGHT);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
                                 &img.endian);

	// 各壁ごとに縦横10pxのブロックを描画
	for(int i = 0; i < sizeof(MAP) / sizeof(MAP[0]); i++){
	  for (int j = 0; j < ft_strlen(MAP[i]); j++){
		printf("%c", MAP[i][j]);
		if (MAP[i][j] == '1'){
			// 正方形を描画
			for (int y = i * 10; y < i * 10 + 10; y++){
				for (int x = j * 10; x < j * 10 + 10; x++){
					my_mlx_pixel_put(&img, x, y, 0x00FFFFFF);
				}
			}
		}
	  }
	  printf("\n");
	}

    mlx_put_image_to_window(vars.mlx, vars.win, img.img, 0, 0);
    mlx_loop(vars.mlx);
}
