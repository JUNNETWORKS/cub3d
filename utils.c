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
	color |= (int)((src & 0x000000ff) * alpha) + (int)((dst & 0x000000ff) * (1 - alpha));
	color |= (int)((src & 0x0000ff00) * alpha) + (int)((dst & 0x0000ff00) * (1 - alpha));
	color |= (int)((src & 0x00ff0000) * alpha) + (int)((dst & 0x00ff0000) * (1 - alpha));
	return (color);
}
