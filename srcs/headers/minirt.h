/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 17:33:27 by craimond          #+#    #+#             */
/*   Updated: 2024/04/24 14:40:32 by craimond         ###   ########.fr       */
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
# include <pthread.h>
# include <sys/time.h>
# include "../../minilibx-linux/mlx.h"
# include "../../libft/libft.h"
# include "../headers/get_next_line.h"
# include "../headers/primitives.h"
# include "utils.h"
# include "scene.h"

//valori ideali
# define WIN_SIZE 0.75
# define WORLD_SIZE 1000
# define OCTREE_DEPTH 2
# define N_THREADS 8

# define BOUNCE_ATTENUATION_FACTOR 0.8
# define BACKGROUND_COLOR 0xc0c6ed
# define KEY_ESC 65307
# define KEY_SPACE 32

# define CHECKERBOARD_TILE_DENSITY 15
# define CHECKERBOARD_COLOR1 0x000000
# define CHECKERBOARD_COLOR2 0xFFFFFF

# ifndef  M_PI
#  define  M_PI  3.1415926535897932384626433
# endif

# define TEXTURE_ROOT "textures/"

static const char		spaces[] = " \t\n\v\f\r";

typedef struct s_mlx_data
{
	void			*win;
	void			*mlx;
	void			**images;
	char			**addrresses;
	double			*viewport_x;
	double			*viewport_y;
	double			aspect_ratio;
	int32_t			win_width;
	int32_t			win_height;
	int32_t			endian;
	int32_t			bits_per_pixel;
	int32_t			line_length;
	int16_t			current_img;
	uint16_t		n_images;
	uint8_t			bytes_per_pixel;
}	t_mlx_data;

typedef struct s_hook_data
{
	t_mlx_data	*win_data;
	t_scene		*scene;
}	t_hook_data;

typedef struct s_thread_data
{
	t_mlx_data	*win_data;
	t_scene		*scene;
	double		*light_ratios;
	uint16_t	start_y;
	uint16_t	end_y;
}	t_thread_data;

typedef struct scene_and_mlx
{
	t_scene		*scene;
	t_mlx_data	*mlx_data;
}	t_scene_and_mlx;

//TODO aggiustare i const
void			check_args(const uint16_t argc, char **argv);
void			init_scene(t_scene *scene);
void 			parse_scene(const int fd, t_scene *scene);
void			setup_scene(t_scene *scene);
void			init_window(t_mlx_data *win_data, t_scene *scene);
void			init_hooks(t_mlx_data *win_data, t_scene *scene);
void			init_textures(const t_scene *scene, t_mlx_data *mlx_data);
void			render_scene(t_mlx_data *win_data, t_scene *scene);
t_hit			*trace_ray(const t_scene *scene, const t_ray ray);
t_color			add_lighting(const t_scene *scene, t_color color, const t_hit *hit_info, const double *light_ratio);
double			intersect_ray_sphere(const t_ray ray, const t_shape *shape);
double			intersect_ray_plane(const t_ray ray, const t_shape *shape);
double			intersect_ray_cylinder(const t_ray ray, const t_shape *shape);
double			intersect_ray_triangle(const t_ray, const t_shape *shape);
double			intersect_ray_cone(const t_ray ray, const t_shape *shape);
bool			ray_intersects_aabb(t_ray ray, t_point bounding_box_max, t_point bounding_box_min);
t_point			ray_point_at_parameter(const t_ray ray, double t);
void			get_uv(const t_hit *hit_info, double *u, double *v);
double			*precompute_ratios(uint16_t n_elems);
void			precompute_viewports(t_mlx_data *win_data);
t_thread_data	**set_threads_data(t_scene_and_mlx s_and_m,
					double *light_ratios, uint16_t lines_per_thread,
					pthread_attr_t *thread_attr);
void			set_thread_attr(pthread_attr_t *thread_attr);
t_color			blend_colors(const t_color color1, const t_color color2, double ratio);
void			setup_camera(t_camera *cam, const t_mlx_data *win_data);
void 			ft_quit(const uint8_t id, char *msg);
void 			destroy_scene(t_scene *scene);
int 			close_win(t_hook_data *hook_data);
void			my_mlx_pixel_put(const t_mlx_data *data, const uint16_t x, const uint16_t y, const t_color color);
t_color 		my_mlx_pixel_get(const t_texture_data *data, const uint32_t x, const uint32_t y);
t_scene			*get_scene(t_scene *_scene);
void			parse_line(char *line, t_scene *scene);
void			parse_amblight(t_scene *scene);
void			parse_light(t_scene *scene);
void			parse_camera(t_scene *scene);
void			parse_shape(char *line, t_scene *scene);
void			parse_material(t_material *material);
void			parse_texture(const char *str, t_material *material);
void			parse_sphere(t_shape *shape);
void			parse_plane(t_shape *shape);
void			parse_cylinder(t_shape *shape);
void			parse_triangle(t_shape *shape);
void			parse_cone(t_shape *shape);
t_float3		parse_coord(char *str);
t_color			parse_color(char *str);
char			*skip_commas(char *str);
bool			is_scene_valid(const t_scene *scene);
void			set_shapes_data(t_scene *scene);
void			set_world_extremes(t_scene *scene);
void			fill_octree(t_octree *node, t_list *shapes,
					uint8_t depth, t_vector bt_bb[2]);
void	update_box_limits(t_point *new_box_top,
					t_point *new_box_bottom, t_vector size, uint8_t i);
t_list	*get_shapes_inside_box(t_list *shapes,
					t_point box_top, t_point box_bottom);
int	boxes_overlap(const t_point box1_top, const t_point box1_bottom,
					const t_point box2_top, const t_point box2_bottom);
void			set_bb_cylinder(t_shape *shape);
void			set_bb_sphere(t_shape *shape);
void			set_bb_plane(t_shape *shape);
void			set_bb_cone(t_shape *shape);
void			set_bb_triangle(t_shape *shape);
double			intersect_cylinder_side(const t_ray ray, const t_cylinder *cylinder, const double dot_ray_cylinder);
void			set_bounding_box(t_shape *shape);

#endif