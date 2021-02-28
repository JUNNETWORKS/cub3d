/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 16:42:52 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 16:43:53 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

uint32_t	alpha_blend(uint32_t dst, uint32_t src)
{
	double		alpha;
	uint32_t	color;

	alpha = -((double)(src >> 24) / (double)0xff - 1.0);
	color = 0x00000000;
	color |= (int)((src & 0xff) * alpha) +
		(int)((dst & 0x000000ff) * (1 - alpha));
	color |= ((int)((src >> 8 & 0xff) * alpha) +
		(int)((dst >> 8 & 0xff) * (1 - alpha))) << 8;
	color |= ((int)((src >> 16 & 0xff) * alpha) +
		(int)((dst >> 16 & 0xff) * (1 - alpha))) << 16;
	return (color);
}

uint32_t	rgb2hex(int r, int g, int b)
{
	uint32_t	color;

	color |= b;
	color |= g << 8;
	color |= r << 16;
	return (color);
}
