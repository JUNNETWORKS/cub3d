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
