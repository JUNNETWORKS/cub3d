#include "./cub3d.h"

double	deg2rad(int x)
{
	return ((double)x / 180 * M_PI);
}

int	rad2deg(double x)
{
	return (int)(x * 180 / M_PI);
}

uint32_t	rgb2hex(char r, char g, char b)
{
	uint32_t color;
	color = b;
	color |= g << 8;
	color |= r << 16;
	return (color);
}

// アルファブレンディング
// dst: 背景, src: 前景
uint32_t	alpha_blend(uint32_t dst, uint32_t src)
{
	// 透明: 0.0, 不透明: 1.0
	double alpha = -((double)(src >> 24) / (double)0xff - 1.0);
	uint32_t color = 0x00000000;
	color |= (int)((src & 0xff) * alpha) + (int)((dst & 0x000000ff) * (1 - alpha));
	color |= ((int)((src >> 8 & 0xff) * alpha) + (int)((dst >> 8 & 0xff) * (1 - alpha))) << 8;
	color |= ((int)((src >> 16 & 0xff) * alpha) + (int)((dst >> 16 & 0xff) * (1 - alpha))) << 16;
	return (color);
}
