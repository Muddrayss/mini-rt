/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:33:18 by craimond          #+#    #+#             */
/*   Updated: 2024/04/24 20:43:58 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

int	main(int argc, char **argv)
{
	t_mlx_data		mlx_data;
	t_scene			scene;
	int				fd;

	init_scene(&scene);
	check_args(argc, argv);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		ft_quit(4, "failed to open scene: no such file or directory\n");
	ft_putstr_fd("Parsing scene...\n", 1);
	parse_scene(fd, &scene);
	setup_scene(&scene);
	init_window(&mlx_data, &scene);
	init_textures(&scene, &mlx_data);
	init_hooks(&mlx_data, &scene);
	gettimeofday(&start_time, NULL);
	render_scene(&mlx_data, &scene);
	gettimeofday(&end_time, NULL);
	mlx_loop(mlx_data.mlx);
	ft_quit(0, "Minilibx loop ended abruptly\n");
}
