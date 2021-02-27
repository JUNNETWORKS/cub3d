#include "cub3d.h"

void		put_error_msg(char *str)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
}

int			put_and_return_err(char *str)
{
	put_error_msg(str);
	return (ERROR);
}
