/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:27:35 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 18:39:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

static void	init_window2(t_mlx_data *win_data)
{
	uint16_t	i;

	i = 0;
	while (i < win_data->n_images)
	{
		win_data->images[i] = mlx_new_image(win_data->mlx,
				win_data->win_width, win_data->win_height);
		if (!win_data->images[i])
			ft_quit(3, "image initialization failed");
		win_data->addrresses[i] = mlx_get_data_addr(win_data->images[i],
				&win_data->bits_per_pixel,
				&win_data->line_length, &win_data->endian);
		i++;
	}
	win_data->current_img = 0;
	win_data->bytes_per_pixel = win_data->bits_per_pixel / 8;
	win_data->aspect_ratio = (double)win_data->win_width
		/ (double)win_data->win_height;
	win_data->viewport_x = (double *)calloc_p(win_data->win_width,
			sizeof(double));
	win_data->viewport_y = (double *)calloc_p(win_data->win_height,
			sizeof(double));
	precompute_viewports(win_data);
}

void	init_window(t_mlx_data *win_data, t_scene *scene)
{
	win_data->mlx = mlx_init();
	mlx_get_screen_size(win_data->mlx,
		&win_data->win_width, &win_data->win_height);
	win_data->win_width *= WIN_SIZE;
	win_data->win_height *= WIN_SIZE;
	win_data->win = mlx_new_window(win_data->mlx,
			win_data->win_width, win_data->win_height, "miniRT");
	if (!win_data->win)
		ft_quit(3, "window initialization failed");
	win_data->n_images = ft_lstsize(scene->cameras);
	win_data->images = (void **)calloc_p(win_data->n_images, sizeof(void *));
	win_data->addrresses = (char **)calloc_p(win_data->n_images,
			sizeof(char *));
	init_window2(win_data);
}
