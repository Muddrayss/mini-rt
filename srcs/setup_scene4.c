/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 18:23:00 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 18:54:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

void	set_bb_sphere(t_shape *shape)
{
	shape->bb_min = (t_point){shape->sphere.center.x - shape->sphere.radius,
		shape->sphere.center.y - shape->sphere.radius,
		shape->sphere.center.z - shape->sphere.radius};
	shape->bb_max = (t_point){shape->sphere.center.x + shape->sphere.radius,
		shape->sphere.center.y + shape->sphere.radius,
		shape->sphere.center.z + shape->sphere.radius};
}

void	update_plane_bb_max_min(t_point *bb_min, t_point *bb_max,
	t_vector normal, t_vector size_by_u_v[2])
{
	t_vector	normal_by_thickness;
	
	bb_min->x = -size_by_u_v[0].x - size_by_u_v[1].x;
	bb_min->y = -size_by_u_v[0].y - size_by_u_v[1].y;
	bb_min->z = -size_by_u_v[0].z - size_by_u_v[1].z;
	bb_max->x = size_by_u_v[0].x + size_by_u_v[1].x;
	bb_max->y = size_by_u_v[0].y + size_by_u_v[1].y;
	bb_max->z = size_by_u_v[0].z + size_by_u_v[1].z;
	normal_by_thickness.x = normal.x * EPSILON;
	normal_by_thickness.y = normal.y * EPSILON;
	normal_by_thickness.z = normal.z * EPSILON;
	bb_min->x -= normal_by_thickness.x;
	bb_min->y -= normal_by_thickness.y;
	bb_min->z -= normal_by_thickness.z;
	bb_max->x += normal_by_thickness.x;
	bb_max->y += normal_by_thickness.y;
	bb_max->z += normal_by_thickness.z;
}

void	set_bb_plane(t_shape *shape)
{
	t_vector				u;
	t_vector				v;
	t_vector				r;
	t_vector				size_by_u_v[2];
	static const double		size = WORLD_SIZE / 2;

	r = (t_vector){1, 0, 0};
	if (fabs(vec_dot(r, shape->plane.normal)) > 0.99f)
		r = (t_vector){0, 1, 0};
	u = vec_cross(r, shape->plane.normal);
	v = vec_cross(u, shape->plane.normal);
	u = vec_normalize(u);
	v = vec_normalize(v);
	size_by_u_v[1].x = size * fabs(v.x);
	size_by_u_v[1].y = size * fabs(v.y);
	size_by_u_v[1].z = size * fabs(v.z);
	size_by_u_v[0].x = size * fabs(u.x);
	size_by_u_v[0].y = size * fabs(u.y);
	size_by_u_v[0].z = size * fabs(u.z);
	update_plane_bb_max_min(&shape->bb_min, &shape->bb_max,
		shape->plane.normal, size_by_u_v);
}
