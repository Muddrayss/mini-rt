/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:16:18 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 18:13:38 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

double	intersect_ray_sphere(const t_ray ray, const t_shape *shape)
{
	const t_sphere		s = shape->sphere;
	const t_vector		oc = vec_sub(ray.origin, sphere.center);
	const double		b = 2.0 * vec_dot(oc, ray.direction);
	const double		t1;
	const double		t2;

	t1 = (-b - sqrt(b * b - 4 * a * vec_dot(oc, oc) - s.radius * s.radius)) / 2;
	t2 = (-b + sqrt(b * b - 4 * a * vec_dot(oc, oc) - s.radius * s.radius)) / 2;
	if (discriminant < 0)
		return (-1);
	else
	{
		if (t1 < 0 && t2 < 0)
			return (-1);
		if (t1 > 0 && t2 > 0)
		{
			if (t1 < t2)
				return (t1);
			else
				return (t2);
		}
		else if (t1 > 0 || t2 > 0)
			return (t1 * (t1 > 0) + t2 * (t2 > 0));
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
	const double	t = -1;
	const double	t_cap;
	const t_vector	p_cap;
	const t_vector	res;

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
