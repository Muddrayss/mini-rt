/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:33:27 by craimond          #+#    #+#             */
/*   Updated: 2024/03/29 19:20:25 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdint.h>
# include <float.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <math.h>
# include <stdint.h>
# include <fcntl.h>
# include "../../minilibx-linux/mlx.h"
# include "../../libft/libft.h"
# include "../headers/get_next_line.h"
# include "scene.h"

# define WIN_HEIGHT 480
# define WIN_WIDTH 640
# define WORLD_SIZE 100
# define KEY_ESC 65307
# define OCTREE_DEPTH 4
# define BACKGROUND_COLOR 0x000000 //così lo puo definire l'utente

static const char		spaces[] = " \t\n\v\f\r";

typedef struct s_mlx_data
{
	void			*win;
	void			*img;
	void			*mlx;
	int				endian;
	int				bits_per_pixel;
	int				line_length;
	char			*addr;
}	t_mlx_data;

typedef struct s_hook_data
{
	t_mlx_data	*win_data;
	t_scene		scene;
}	t_hook_data;

//TODO aggiustare i const
void			check_args(const uint16_t argc, char **argv);
void			init_scene(t_scene *scene);
void			init_window(t_mlx_data *win_data);
void			init_hooks(t_mlx_data *win_data, t_scene scene);
void			parse_scene(int fd, t_scene *scene);
void			set_bounding_box(t_shape *shape);
void			setup_scene(t_scene *scene);
void			render(const t_mlx_data mlx_data, t_scene scene);
void			my_mlx_pixel_put(const t_mlx_data data, const uint16_t x, const uint16_t y, const t_color color);
void 			ft_quit(uint8_t id, char *msg);
int				close_win(t_hook_data *hook_data);
bool			is_empty_line(const char *line);
bool			is_comment(const char *line);
bool			is_space(char c);
float 			ft_atof(const char *str);
uint8_t 		ft_atoui(const char *str);
char 			*ft_strtok(char *const str, const char *const sep);
bool			ray_intersects_aabb(t_ray ray, t_point bounding_box_max, t_point bounding_box_min);

#endif