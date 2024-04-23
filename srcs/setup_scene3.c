/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 18:01:45 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 18:49:59 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

static void	update_cylinder_bb_max_min(t_point *bb_min, t_point *bb_max,
	t_vector axis_min_max[2], t_vector radius_extents)
{
	bb_min->x = fmin(axis_min_max[0].x, axis_min_max[1].x) - radius_extents.x;
	bb_min->y = fmin(axis_min_max[0].y, axis_min_max[1].y) - radius_extents.y;
	bb_min->z = fmin(axis_min_max[0].z, axis_min_max[1].z) - radius_extents.z;
	bb_max->x = fmax(axis_min_max[0].x, axis_min_max[1].x) + radius_extents.x;
	bb_max->y = fmax(axis_min_max[0].y, axis_min_max[1].y) + radius_extents.y;
	bb_max->z = fmax(axis_min_max[0].z, axis_min_max[1].z) + radius_extents.z;
}

void	set_bb_cylinder(t_shape *shape)
{
	const t_vector	orien = shape->cylinder.direction;
	const double	r = shape->cylinder.radius;
	t_vector		perps[2];
	t_vector		amm[2];
	t_vector		r_extents;

	if (fabs(orien.x) < fabs(orien.y)
		|| fabs(orien.x) < fabs(orien.z))
		perps[0] = (t_vector){0, -orien.z, orien.y};
	else
		perps[0] = (t_vector){-orien.y, orien.x, 0};
	perps[0] = vec_normalize(perps[0]);
	perps[1] = vec_cross(orien, perps[0]);
	amm[1].x = shape->cylinder.center.x + orien.x * shape->cylinder.half_height;
	amm[1].y = shape->cylinder.center.y + orien.y * shape->cylinder.half_height;
	amm[1].z = shape->cylinder.center.z + orien.z * shape->cylinder.half_height;
	amm[0].x = shape->cylinder.center.x - orien.x * shape->cylinder.half_height;
	amm[0].y = shape->cylinder.center.y - orien.y * shape->cylinder.half_height;
	amm[0].z = shape->cylinder.center.z - orien.z * shape->cylinder.half_height;
	r_extents.x = r * sqrt(perps[0].x * perps[0].x + perps[1].x * perps[1].x);
	r_extents.y = r * sqrt(perps[0].y * perps[0].y + perps[1].y * perps[1].y);
	r_extents.z = r * sqrt(perps[0].z * perps[0].z + perps[1].z * perps[1].z);
	update_cylinder_bb_max_min(&shape->bb_min, &shape->bb_max,
		amm, r_extents);
}

void	set_bb_cone(t_shape *shape)
{
	const t_vector	bc = shape->cone.base_center;
	const double	r = shape->cone.radius;
	t_vector		p[2];
	t_vector		tip;
	t_vector		r_extents;

	tip.x = bc.x + shape->cone.direction.x * shape->cone.height;
	tip.y = bc.y + shape->cone.direction.y * shape->cone.height;
	tip.z = bc.z + shape->cone.direction.z * shape->cone.height;
	if (fabs(shape->cone.direction.x) < fabs(shape->cone.direction.y)
		|| fabs(shape->cone.direction.x) < fabs(shape->cone.direction.z))
		p[0] = (t_vector){0, -shape->cone.direction.z, shape->cone.direction.y};
	else
		p[0] = (t_vector){-shape->cone.direction.y, shape->cone.direction.x, 0};
	p[0] = vec_normalize(p[0]);
	p[1] = vec_cross(shape->cone.direction, p[0]);
	r_extents.x = r * sqrt(p[0].x * p[0].x + p[1].x * p[1].x);
	r_extents.y = r * sqrt(p[0].y * p[0].y + p[1].y * p[1].y);
	r_extents.z = r * sqrt(p[0].z * p[0].z + p[1].z * p[1].z);
	shape->bb_min.x = fmin(fmin(bc.x, tip.x) - r_extents.x, bc.x - r);
	shape->bb_min.y = fmin(fmin(bc.y, tip.y) - r_extents.y, bc.y - r);
	shape->bb_min.z = fmin(fmin(bc.z, tip.z) - r_extents.z, bc.z - r);
	shape->bb_max.x = fmax(fmax(bc.x, tip.x) + r_extents.x, bc.x + r);
	shape->bb_max.y = fmax(fmax(bc.y, tip.y) + r_extents.y, bc.y + r);
	shape->bb_max.z = fmax(fmax(bc.z, tip.z) + r_extents.z, bc.z + r);
}

void	set_bb_triangle(t_shape *shape)
{
	const t_vector	v0 = shape->triangle.vertices[0];
	const t_vector	v1 = shape->triangle.vertices[1];
	const t_vector	v2 = shape->triangle.vertices[2];
	t_vector		min;
	t_vector		max;

	min = v0;
	max = v0;
	min.x = fmin(min.x, v1.x);
	min.y = fmin(min.y, v1.y);
	min.z = fmin(min.z, v1.z);
	max.x = fmax(max.x, v1.x);
	max.y = fmax(max.y, v1.y);
	max.z = fmax(max.z, v1.z);
	min.x = fmin(min.x, v2.x);
	min.y = fmin(min.y, v2.y);
	min.z = fmin(min.z, v2.z);
	max.x = fmax(max.x, v2.x);
	max.y = fmax(max.y, v2.y);
	max.z = fmax(max.z, v2.z);
	shape->bb_min = (t_point)min;
	shape->bb_max = (t_point)max;
}
