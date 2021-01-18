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

void	draw_block(t_game *game, int x, int y, int color){
	// 正方形を描画
	for (int xx = 0; xx < BLOCK_SIZE; xx++){
		for (int yy = 0; yy < BLOCK_SIZE; yy++){
			my_mlx_pixel_put(game, x + xx, y + yy, color);
		}
	}
}

void	draw_wall(t_game *game)
{
	// 各壁ごとに縦横10pxのブロックを描画
	for(int i = 0; i < sizeof(MAP) / sizeof(MAP[0]); i++){
	  for (int j = 0; j < ft_strlen(MAP[i]); j++){
		printf("%c", MAP[i][j]);
		if (MAP[i][j] == '1'){
			draw_block(game, j * BLOCK_SIZE, i * BLOCK_SIZE, 0x00FFFFFF);
		}
		if (ft_strchr("NSEW", MAP[i][j])){
			draw_block(game, j * BLOCK_SIZE, i * BLOCK_SIZE, 0x00FF0000);
		}
	  }
	  printf("\n");
	}
}

int		main_loop(t_game *game)
{
	draw_wall(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img, 0, 0);
	return (0);
}

int main(int argc, char **argv){
	t_game	game;
	initialize_game(&game);

	mlx_key_hook(game.win, key_hook, &game);
    mlx_put_image_to_window(game.mlx, game.win, game.img.img, 0, 0);
	mlx_loop_hook(game.mlx, &main_loop, &game);
    mlx_loop(game.mlx);
}
