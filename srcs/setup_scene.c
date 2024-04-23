/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:35:08 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 18:51:14 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

int	boxes_overlap(const t_point box1_top, const t_point box1_bottom,
	const t_point box2_top, const t_point box2_bottom)
{
	return (box1_bottom.x <= box2_top.x && box1_top.x >= box2_bottom.x
		&& box1_bottom.y <= box2_top.y && box1_top.y >= box2_bottom.y
		&& box1_bottom.z <= box2_top.z && box1_top.z >= box2_bottom.z);
}

void	setup_scene(t_scene *scene)
{
	t_point		bt_bb[2];

	bt_bb[0] = scene->world_max;
	bt_bb[1] = scene->world_min;
	set_shapes_data(scene);
	set_world_extremes(scene);
	fill_octree(scene->octree, scene->shapes,
		OCTREE_DEPTH, bt_bb);
}

void	update_box_limits(t_point *new_box_top,
	t_point *new_box_bottom, t_vector size, uint8_t i)
{
	if (i & 1)
		new_box_top->x += size.x;
	else
		new_box_bottom->x -= size.x;
	if (i & 2)
		new_box_top->y += size.y;
	else
		new_box_bottom->y -= size.y;
	if (i & 4)
		new_box_top->z += size.z;
	else
		new_box_bottom->z -= size.z;
}

void	set_bounding_box(t_shape *shape)
{
	static void (*const	get_bb_funcs[])(t_shape *) = {&set_bb_sphere,
		&set_bb_cylinder, &set_bb_triangle, &set_bb_cone, &set_bb_plane};
	const uint8_t		n_shapes = sizeof(get_bb_funcs)
		/ sizeof(get_bb_funcs[0]);
	uint8_t				i;

	i = 0;
	while (i < n_shapes)
	{
		if (shape->e_type == i)
		{
			get_bb_funcs[i](shape);
			return ;
		}
		i++;
	}
}
