/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:16:18 by craimond          #+#    #+#             */
/*   Updated: 2024/04/24 14:45:53 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

double	intersect_cylinder_side(const t_ray ray, const t_cylinder *cylinder, const double dot_ray_cylinder)
{
	const t_vector	oc = vec_sub(ray.origin, cylinder->center);
	const double	dot_oc_cylinder = vec_dot(oc, cylinder->direction);
	const double	A = 1 - pow(dot_ray_cylinder, 2);
	const double	B = 2 * (vec_dot(ray.direction, oc) - (dot_ray_cylinder * dot_oc_cylinder));
	const double 	C = vec_dot(oc, oc) - pow(dot_oc_cylinder, 2) - cylinder->squared_radius;
	const double	discriminant = B * B - 4 * A * C;
	if (discriminant < 0)
		return (-1);
	const double	sqrt_discriminant = sqrt(discriminant);
	const double	two_times_a = 2 * A;
	const double	t[2] = {
		(-B - sqrt_discriminant) / two_times_a,
		(-B + sqrt_discriminant) / two_times_a
	};
	double	valid_t = -1;
	uint8_t	i = 0;
	while (i < 2)
	{
		if (t[i] > 0)
		{
			const t_point	point = vec_add(ray.origin, vec_scale(t[i], ray.direction));
			const t_vector	vec_from_center_to_point = vec_sub(point, cylinder->center);
			const double	projection_lenght = vec_dot(vec_from_center_to_point, cylinder->direction);

			if (fabs(projection_lenght) <= cylinder->half_height)
			{
				valid_t = t[i];
				break ;
			}
		}
		i++;
	}
	return (valid_t);
}

double	intersect_ray_triangle(const t_ray ray, const t_shape *shape)
{
	const t_vector	e[2] = {
		vec_sub(shape->triangle.vertices[1], shape->triangle.vertices[0]),
		vec_sub(shape->triangle.vertices[2], shape->triangle.vertices[0])
	};
	const t_vector	h = vec_cross(ray.direction, e[1]);
	double			u;
	t_vector		q;
	double			v;

	if (vec_dot(e[0], h) > -EPSILON && vec_dot(e[0], h) < EPSILON)
		return (-1);
	u = 1.0f / vec_dot(e[0], h)
		* vec_dot(vec_sub(ray.origin, shape->triangle.vertices[0]), h);
	if (u < 0.0f || u > 1.0f)
		return (-1);
	q = vec_cross(vec_sub(ray.origin, shape->triangle.vertices[0]), e[0]);
	v = 1.0f / vec_dot(e[0], h) * vec_dot(ray.direction, q);
	if (v < 0.0f || u + v > 1.0f)
		return (-1);
	if (1.0f / vec_dot(e[0], h) * vec_dot(e[1], q) > EPSILON)
		return (1.0f / vec_dot(e[0], h) * vec_dot(e[1], q));
	return (-1);
}

double	intersect_ray_cone(const t_ray r, const t_shape *shape)
{
	const t_cone		cone = shape->cone;
	const t_vector		c = cone.base_center;
	const double		abc[3] = {
	[0] = vec_dot(r.direction, cone.direction)
		* vec_dot(r.direction, cone.direction) - cone.costheta_squared,
	[1] = 2.0 * (vec_dot(r.direction, cone.direction) * vec_dot(vec_sub(
					r.origin, c), cone.direction) - vec_dot(r.direction,
				vec_sub(r.origin, c)) * cone.costheta_squared),
	[2] = vec_dot(vec_sub(r.origin, c), cone.direction)
		* vec_dot(vec_sub(r.origin, c), cone.direction) - vec_dot(
			vec_sub(r.origin, c), vec_sub(r.origin, c)) * cone.costheta_squared
	};
	double				ts[2];
	double				discriminant;

	discriminant = abc[1] * abc[1] - 4 * abc[0] * abc[2];
	if (discriminant < 0)
		return (-1.0);
	ts[0] = (-abc[1] - sqrt(discriminant)) / (2 * abc[0]);
	ts[1] = (-abc[1] + sqrt(discriminant)) / (2 * abc[0]);
	if (ts[0] > 0 && ts[1] > 0)
		return (fmin(ts[0], ts[1]));
	else if (ts[0] > 0 || ts[1] > 0)
		return (ts[0] * (ts[0] > 0) + ts[1] * (ts[1] > 0));
	else
		return (-1.0);
}

static double	get_tmin_tmax(double t0, double t1)
{
	double	temp;

	if (t0 > t1)
	{
		temp = t0;
		t0 = t1;
		t1 = temp;
	}
	return (t0);
}

inline bool	ray_intersects_aabb(t_ray r, t_point b_max, t_point b_min)
{
	const double	r_direction[3]
		= {r.direction.x, r.direction.y, r.direction.z};
	const double	bb_max[3] = {b_max.x, b_max.y, b_max.z};
	const double	bb_min[3] = {b_min.x, b_min.y, b_min.z};
	const double	r_origin[3] = {r.origin.x, r.origin.y, r.origin.z};
	double			ts[2];
	uint8_t			i;

	i = 0;
	while (i < 3)
	{
		ts[0] = (bb_min[i] - r_origin[i]) * 1.0f / r_direction[i];
		ts[1] = (bb_max[i] - r_origin[i]) * 1.0f / r_direction[i];
		if (1.0f / r_direction[i] < 0.0f)
		{
			double tmp = ts[0];
			ts[0] = ts[1];
			ts[1] = tmp;
		}
		if (get_tmin_tmax(ts[1], FLT_MAX) <= get_tmin_tmax(ts[0], -FLT_MAX))
			return (false);
		i++;
	}
	return (true);
}

inline t_point	ray_point_at_parameter(const t_ray ray, double t)
{
	return ((t_point)
		{
			.x = ray.origin.x + t * ray.direction.x,
			.y = ray.origin.y + t * ray.direction.y,
			.z = ray.origin.z + t * ray.direction.z
		});
}
