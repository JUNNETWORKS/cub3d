/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtanaka <jtanaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/28 23:57:46 by jtanaka           #+#    #+#             */
/*   Updated: 2021/02/28 23:58:45 by jtanaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	has_texture_loaded(t_game *game, char *name)
{
	if (ft_strnstr(name, "NO", ft_strlen(name)) && game->tex_n.img)
		return (true);
	else if (ft_strnstr(name, "SO", ft_strlen(name)) && game->tex_s.img)
		return (true);
	else if (ft_strnstr(name, "WE", ft_strlen(name)) && game->tex_w.img)
		return (true);
	else if (ft_strnstr(name, "EA", ft_strlen(name)) && game->tex_e.img)
		return (true);
	else if (name[0] == 'S' && game->tex_sprite.img)
		return (true);
	return (false);
}

int			load_texture(t_game *game, char *name, char *texture_path)
{
	t_img *tex;

	printf("name: %s, texture_path: %s\n", name, texture_path);
	if (has_texture_loaded(game, name))
		return (put_and_return_err("Duplicated texture key"));
	if (ft_strnstr(name, "NO", ft_strlen(name)))
		tex = &game->tex_n;
	else if (ft_strnstr(name, "SO", ft_strlen(name)))
		tex = &game->tex_s;
	else if (ft_strnstr(name, "WE", ft_strlen(name)))
		tex = &game->tex_w;
	else if (ft_strnstr(name, "EA", ft_strlen(name)))
		tex = &game->tex_e;
	else if (name[0] == 'S')
		tex = &game->tex_sprite;
	if (load_image(game, tex, texture_path) == ERROR)
		return (put_and_return_err("Failed to load texture"));
	return (0);
}
