#include "./cub3d.h"

double	deg2rad(int x)
{
	return ((double)x / 180 * M_PI);
}

int	rad2deg(double x)
{
	return (int)(x * 180 / M_PI);
}
