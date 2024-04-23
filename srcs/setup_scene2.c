/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 17:58:02 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 18:52:34 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

static void	set_cylinder_data(t_shape *shape)
{
	t_cylinder	*cylinder;
	t_vector	center_to_cap;

	cylinder = &shape->cylinder;
	cylinder->squared_radius = cylinder->radius * cylinder->radius;
	cylinder->direction = vec_normalize(cylinder->direction);
	cylinder->half_height = cylinder->height / 2;
	center_to_cap = vec_scale(cylinder->half_height,
			cylinder->direction);
	cylinder->top_cap_center = vec_add(cylinder->center, center_to_cap);
	cylinder->bottom_cap_center = vec_sub(cylinder->center,
			center_to_cap);
}

static void	set_cone_shape(t_shape *shape)
{
	t_cone		*cone;
	double		cos_theta;

	cone = &shape->cone;
	cos_theta = cos(atan2(shape->cone.radius, shape->cone.height));
	shape->cone.costheta_squared = cos_theta * cos_theta;
	shape->cone.direction = vec_normalize(shape->cone.direction);
}

void	set_shapes_data(t_scene *scene)
{
	t_shape		*shape;
	t_list		*node;

	node = scene->shapes;
	while (node)
	{
		shape = (t_shape *)node->content;
		set_bounding_box(shape);
		if (shape->e_type == TRIANGLE)
			shape->triangle.normal = vec_normalize(vec_cross(
						vec_sub(shape->triangle.vertices[1],
							shape->triangle.vertices[0]),
						vec_sub(shape->triangle.vertices[2],
							shape->triangle.vertices[0])));
		else if (shape->e_type == PLANE)
			shape->plane.normal = vec_normalize(shape->plane.normal);
		else if (shape->e_type == CYLINDER)
			set_cylinder_data(shape);
		else if (shape->e_type == CONE)
			set_cone_shape(shape);
		node = node->next;
	}
}

void	set_world_extremes(t_scene *scene)
{
	t_list		*shapes;
	t_shape		*shape;
	t_point		world_max;
	t_point		world_min;

	world_max = (t_point){0, 0, 0};
	world_min = (t_point){0, 0, 0};
	shapes = scene->shapes;
	if (!shapes)
		return ;
	shape = (t_shape *)shapes->content;
	while (shapes)
	{
		shape = shapes->content;
		world_max = (t_point){fmax(world_max.x, shape->bb_max.x),
			fmax(world_max.y, shape->bb_max.y),
			fmax(world_max.z, shape->bb_max.z)};
		world_min = (t_point){fmin(world_min.x, shape->bb_min.x),
			fmin(world_min.y, shape->bb_min.y),
			fmin(world_min.z, shape->bb_min.z)};
		shapes = shapes->next;
	}
	scene->world_min = world_min;
	scene->world_max = world_max;
}
