#include "./cub3d.h"

double	deg2rad(int x)
{
	return ((double)x / 180 * M_PI);
}

int	rad2deg(double x)
{
	return (int)(x * 180 / M_PI);
}

bool		str_all_true(char *str, int(*f)(int))
{
	int		i;

	if (!str)
		return (false);
	i = 0;
	while (str[i])
	{
		if (!f(str[i]))
			return (false);
		i++;
	}
	return (true);
}

size_t		str_c_count(char *str, char c)
{
	int		i;
	int		count;

	i = 0;
	count = 0;
	while(str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}
