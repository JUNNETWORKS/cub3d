#include "cub3d.h"

void	free_and_assign_null(void **p)
{
	free(*p);
	*p = NULL;
}

