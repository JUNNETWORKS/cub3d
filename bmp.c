#include "cub3d.h"

int	write_game2bmp(t_game *game, char *filepath)
{
	int fd;
	if ((fd = open(filepath, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
		return (-1);
	
	// File Header
	// bfType
	write(fd, "BM", 2);
	// bfSize  ファイルサイズ[byte]
	uint32_t filesize = 14 + 10 + game->screen_width * game->screen_height * 4;  // FileHeader + InformationHeader + imagedata
	write(fd, &filesize, 4);
	// bfReserved1
	write(fd, "\0\0", 2);
	// bfReserved2
	write(fd, "\0\0", 2);
	// bfOffBits  ファイル先頭から画像データまでのオフセット[byte]
	uint32_t bfOffBits = 14 + 10;
	write(fd, &bfOffBits, 4);
	
	// Information Header
	// bcSize
	uint32_t a = 12;
	write(fd, &a, 4);
	// bcWidth  画像の幅[ピクセル]
	write(fd, &game->screen_width, 2);
	// bcHeight 画像の高さ[ピクセル]
	write(fd, &game->screen_height, 2);
	// bcPlanes
	a = 1;
	write(fd, &a, 2);
	// bcBitCount
	a = 32;
	write(fd, &a, 2);
	
	// Image Data
	for (int y = game->screen_height - 1; y >= 0; y--){
	  for (int x = 0; x < game->screen_width; x++){
			uint32_t color = get_color_from_img(game->img, x, y);
			write(fd, &color, 4);
	  }
	}
	close(fd);
	return (0);
}
