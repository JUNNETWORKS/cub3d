/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 18:10:23 by jtanaka           #+#    #+#             */
/*   Updated: 2021/03/01 00:18:56 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int			put_and_exit_err(char *str)
{
	put_error_msg(str);
	exit(EXIT_FAILURE);
}
