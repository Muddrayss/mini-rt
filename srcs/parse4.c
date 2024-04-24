/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:30:50 by craimond          #+#    #+#             */
/*   Updated: 2024/04/24 14:31:42 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

void	parse_sphere(t_shape *shape)
{
	shape->sphere.center = parse_coord(ft_strtok(NULL, spaces));
	shape->sphere.radius = ft_atof(ft_strtok(NULL, spaces)) / 2.0f;
	if (shape->sphere.radius <= 0)
		ft_quit(5, "invalid sphere radius");
	shape->e_type = SPHERE;
}

void	parse_plane(t_shape *shape)
{
	shape->plane.center = parse_coord(ft_strtok(NULL, spaces));
	shape->plane.normal = parse_coord(ft_strtok(NULL, spaces));
	shape->e_type = PLANE;
}

void	parse_cylinder(t_shape *shape)
{
	shape->cylinder.center = parse_coord(ft_strtok(NULL, spaces));
	shape->cylinder.direction = parse_coord(ft_strtok(NULL, spaces));
	shape->cylinder.radius = ft_atof(ft_strtok(NULL, spaces)) / 2.0f;
	shape->cylinder.height = ft_atof(ft_strtok(NULL, spaces));
	if (shape->cylinder.radius <= 0)
		ft_quit(5, "invalid cylinder radius");
	if (shape->cylinder.height <= 0)
		ft_quit(5, "invalid cylinder height");
	shape->e_type = CYLINDER;
}

void	parse_cone(t_shape *shape)
{
	shape->cone.base_center = parse_coord(ft_strtok(NULL, spaces));
	shape->cone.direction = parse_coord(ft_strtok(NULL, spaces));
	shape->cone.radius = ft_atof(ft_strtok(NULL, spaces)) / 2.0f;
	shape->cone.height = ft_atof(ft_strtok(NULL, spaces));
	if (shape->cone.radius <= 0)
		ft_quit(5, "invalid cone radius");
	if (shape->cone.height <= 0)
		ft_quit(5, "invalid cone height");
	shape->e_type = CONE;
}

void	parse_triangle(t_shape *shape)
{
	shape->triangle.vertices[0] = parse_coord(ft_strtok(NULL, spaces));
	shape->triangle.vertices[1] = parse_coord(ft_strtok(NULL, spaces));
	shape->triangle.vertices[2] = parse_coord(ft_strtok(NULL, spaces));
	shape->triangle.u[0] = 0;
	shape->triangle.v[0] = 0;
	shape->triangle.u[1] = 1;
	shape->triangle.v[1] = 0;
	shape->triangle.u[2] = 0;
	shape->triangle.v[2] = 1;
	shape->e_type = TRIANGLE;
}
