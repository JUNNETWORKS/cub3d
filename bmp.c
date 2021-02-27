#include "cub3d.h"

int	write_game2bmp(t_game *game, char *fpath)
{
	int			fd;
	uint32_t	len;
	uint32_t	tmp;
	int			x;
	int			y;

	if ((fd = open(fpath, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
		return (-1);
	
	// File Header
	// bfType
	write(fd, "BM", 2);
	// bfSize  ファイルサイズ[byte]
	len = 14 + 10 + game->screen_width * game->screen_height * 4;  // FileHeader + InformationHeader + imagedata
	write(fd, &len, 4);
	// bfReserved1
	write(fd, "\0\0", 2);
	// bfReserved2
	write(fd, "\0\0", 2);
	// bfOffBits  ファイル先頭から画像データまでのオフセット[byte]
	len = 14 + 10;
	write(fd, &len, 4);
	
	// Information Header
	// bcSize
	tmp = 12;
	write(fd, &tmp, 4);
	// bcWidth  画像の幅[ピクセル]
	write(fd, &game->screen_width, 2);
	// bcHeight 画像の高さ[ピクセル]
	write(fd, &game->screen_height, 2);
	// bcPlanes
	tmp = 1;
	write(fd, &tmp, 2);
	// bcBitCount
	tmp = 32;
	write(fd, &tmp, 2);
	
	// Image Data
	y = game->screen_height - 1;
	while (y >= 0)
	{
		x = 0;
		while (x < game->screen_width)
		{
			tmp = get_color_from_img(game->img, x, y);
			write(fd, &tmp, 4);
			x++;
		}
		y--;
	}
	close(fd);
	return (0);
}
