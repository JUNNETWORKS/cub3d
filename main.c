#include "cub3d.h"

char *MAP[] = {
	"1111111111111111111111111",
	"1000000000110000000000001",
	"1011000001110000002000001",
	"100100000000000000000000111111111",
	"111111111011000001110000000000001",
	"100000000011000001110111110111111",
	"11110111111111011100000010001",
	"11110111111111011101010010001",
	"11000000110101011100000010001",
	"10002000000000001100000010001",
	"10000000000000001101010010001",
	"11000001110101011101011010N0111",
	"11110111 1110101 101111010001",
	"11111111 1111111 111111111111",
};

int WIDTH = 1920;
int HEIGHT = 1080;
int BLOCK_SIZE = 20;


// キーボード入力のフック
int 			key_hook(int keycode, t_game *vars)
{
	printf("Pressed Key Code: %d\n", keycode);
	if (keycode == KEY_q || keycode == KEY_esc){
		mlx_destroy_window(vars->mlx, vars->win);
		exit(0);
	}
}

int		main_loop(t_game *game)
{

	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int main(){
	t_game	game;

    game.mlx = mlx_init();
    game.win = mlx_new_window(game.mlx, WIDTH, HEIGHT, "Hello world!");
    game.img.img = mlx_new_image(game.mlx, WIDTH, HEIGHT);
    game.img.addr = mlx_get_data_addr(game.img.img, &game.img.bits_per_pixel, &game.img.line_length, &game.img.endian);

	// 各壁ごとに縦横10pxのブロックを描画
	for(int i = 0; i < sizeof(MAP) / sizeof(MAP[0]); i++){
	  for (int j = 0; j < ft_strlen(MAP[i]); j++){
		printf("%c", MAP[i][j]);
		if (MAP[i][j] == '1'){
			// 正方形を描画
			for (int y = i * BLOCK_SIZE; y < i * BLOCK_SIZE + BLOCK_SIZE; y++){
				for (int x = j * BLOCK_SIZE; x < j * BLOCK_SIZE + BLOCK_SIZE; x++){
					my_mlx_pixel_put(&game, x, y, 0x00FFFFFF);
				}
			}
		}
	  }
	  printf("\n");
	}

	mlx_key_hook(game.win, key_hook, &game);
    mlx_put_image_to_window(game.mlx, game.win, game.img.img, 0, 0);
	mlx_loop_hook(game.mlx, &main_loop, &game);
    mlx_loop(game.mlx);
}
