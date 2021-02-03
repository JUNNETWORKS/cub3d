#include "./cub3d.h"

double	vec2_length(t_vec2 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

void	vec2_add(t_vec2 *a, t_vec2 b)
{
	a->x += b.x;
	a->y += b.y;
}

void	vec2_mult(t_vec2 *vec, int num)
{
	vec->x *= num;
	vec->y *= num;
}

void	vec2_rotate(t_vec2 *vec, double rad)
{
	vec->x = vec->x * cos(rad) - vec->y * sin(rad);
	vec->y = vec->x * sin(rad) + vec->y * cos(rad);
}
