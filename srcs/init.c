/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:27:35 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:17:39 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

static int	key_hook(int keycode, t_hook_data *hook_data);

void	check_args(const uint16_t argc, char **argv)
{
	if (argc != 2)
		ft_quit(1, "invalid number of arguments");
	if (ft_strncmp(argv[1] + ft_strlen(argv[1]) - 3, ".rt", 4) != 0)
		ft_quit(2, "invalid file extension");
}

void	init_scene(t_scene *scene)
{
	get_scene(scene);
	scene->amblight = NULL;
	scene->current_camera = NULL;
	scene->cameras = NULL;
	scene->lights = NULL;
	scene->n_lights = 0;
	scene->shapes = NULL;
	scene->octree = (t_octree *)calloc_p(1, sizeof(t_octree));
	scene->octree->children = NULL;
	scene->octree->shapes = NULL;
	scene->octree->box_bottom = (t_vector){0, 0, 0};
	scene->octree->box_top = (t_vector){0, 0, 0};
	scene->world_max.x = 0;
	scene->world_max.y = 0;
}

void	init_textures(const t_scene *scene, t_mlx_data *md)
{
	t_list			*shapes;
	t_shape			*shape;
	t_material		*m;
	void			*ti;
	int32_t			bits_per_pixel;

	shapes = scene->shapes;
	while (shapes)
	{
		shape = shapes->content;
		m = shape->material;
		if (m->texture)
		{
			ti = mlx_xpm_file_to_image(md->mlx, m->texture->path,
					&m->texture->width, &m->texture->height);
			if (!ti)
				ft_quit(41, "mlx: failed to load texture");
			m->texture->addr = mlx_get_data_addr(ti, &bits_per_pixel,
					&m->texture->line_length, &m->texture->endian);
			if (!m->texture->addr)
				ft_quit(42, "mlx: failed to get texture address");
			m->texture->bytes_per_pixel = bits_per_pixel / 8;
		}
		shapes = shapes->next;
	}
}

void	init_hooks(t_mlx_data *win_data, t_scene *scene)
{
	t_hook_data	*hook_data;

	hook_data = calloc_p(1, sizeof(t_hook_data));
	if (!hook_data)
		ft_quit(3, "hook data initialization failed");
	hook_data->win_data = win_data;
	hook_data->scene = scene;
	mlx_hook(win_data->win, 2, 1L << 0, key_hook, hook_data);
	mlx_hook(win_data->win, 17, 1L << 17, close_win, hook_data);
}

static int	key_hook(const int keycode, t_hook_data *hook_data)
{
	t_mlx_data	*win_data;

	win_data = hook_data->win_data;
	if (keycode == KEY_ESC)
		close_win(hook_data);
	else if (keycode == KEY_SPACE && win_data->n_images > 1)
	{
		win_data->current_img--;
		if (win_data->current_img < 0)
			win_data->current_img = win_data->n_images - 1;
		mlx_clear_window(hook_data->win_data->mlx, hook_data->win_data->win);
		mlx_put_image_to_window(win_data->mlx, win_data->win,
			win_data->images[win_data->current_img], 0, 0);
	}
	return (0);
}
