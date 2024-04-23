/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:16:18 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 19:23:17 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

static double	intersect_cylinder_caps(const t_ray ray,
					const t_cylinder *cylinder, const double dot_ray_cylinder);
static double	intersect_cylinder_cap(const t_ray ray,
					const t_cylinder *cylinder,
					const t_vector extreme_center,
					const double dot_ray_cylinder);

double	intersect_ray_sphere(const t_ray ray, const t_shape *shape)
{
	const t_sphere		s = shape->sphere;
	const t_vector		oc = vec_sub(ray.origin, shape->sphere.center);
	const double		b = 2.0 * vec_dot(oc, ray.direction);
	double				ts[2];
	const double		di = b * b - 4 * vec_dot(oc, oc) + s.radius * s.radius;

	ts[0] = (-b - sqrt(di)) / 2;
	ts[1] = (-b + sqrt(di)) / 2;
	if (di < 0)
		return (-1);
	else
	{
		if (ts[0] < 0 && ts[1] < 0)
			return (-1);
		if (ts[0] > 0 && ts[1] > 0)
		{
			if (ts[0] < ts[1])
				return (ts[0]);
			else
				return (ts[1]);
		}
		else if (ts[0] > 0 || ts[1] > 0)
			return (ts[0] * (ts[0] > 0) + ts[1] * (ts[1] > 0));
		return (0);
	}
}

double	intersect_ray_plane(const t_ray ray, const t_shape *shape)
{
	const t_plane	plane = shape->plane;
	double			denom;
	double			t;

	denom = vec_dot(plane.normal, ray.direction);
	if (fabs(denom) > EPSILON)
	{
		t = vec_dot(vec_sub(plane.center, ray.origin), plane.normal) / denom;
		if (t >= 0)
			return (t);
	}
	return (-1);
}

double	intersect_ray_cylinder(const t_ray ray, const t_shape *shape)
{
	const double	dot_ray_cylinder
		= vec_dot(ray.direction, shape->cylinder.direction);
	const double	t1
		= intersect_cylinder_side(ray, &shape->cylinder, dot_ray_cylinder);
	const double	t2
		= intersect_cylinder_caps(ray, &shape->cylinder, dot_ray_cylinder);

	if (t2 > 0 && (t1 < 0 || t2 < t1))
		return (t2);
	return (t1);
}

static double	intersect_cylinder_caps(const t_ray ray,
	const t_cylinder *cylinder, const double dot_ray_cylinder)
{
	const double	t1 = intersect_cylinder_cap(ray, cylinder,
			cylinder->top_cap_center, dot_ray_cylinder);
	const double	t2 = intersect_cylinder_cap(ray, cylinder,
			cylinder->bottom_cap_center, dot_ray_cylinder);

	if (t1 > 0 && (t1 < t2 || t2 < 0))
		return (t1);
	return (t2);
}

static double	intersect_cylinder_cap(const t_ray ray, const t_cylinder *cylinder, const t_vector extreme_center, const double dot_ray_cylinder)
{
	const double	denom = dot_ray_cylinder;
	double			t;
	double			t_cap;
	t_vector		p_cap;
	t_vector		res;

	t = -1;
	if (fabs(denom) > EPSILON)
	{
		t_cap = vec_dot(vec_sub(extreme_center, ray.origin),
				cylinder->direction) / denom;
		p_cap = vec_add(ray.origin,
				vec_scale(t_cap, ray.direction));
		res = vec_sub(p_cap, extreme_center);
		if (vec_dot(res, res) <= cylinder->squared_radius && t_cap > 0)
			t = t_cap;
	}
	return (t);
}
