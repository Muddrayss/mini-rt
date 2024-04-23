/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:52:38 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:49:31 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

inline t_vector	vec_add(const t_vector a, const t_vector b)
{
	return ((t_vector){a.x + b.x, a.y + b.y, a.z + b.z});
}

inline t_vector	vec_sub(const t_vector a, const t_vector b)
{
	return ((t_vector){a.x - b.x, a.y - b.y, a.z - b.z});
}

inline t_vector	vec_mul(const t_vector a, const t_vector b)
{
	return ((t_vector){a.x * b.x, a.y * b.y, a.z * b.z});
}

inline t_vector	vec_div(const t_vector a, const t_vector b)
{
	return ((t_vector){a.x / b.x, a.y / b.y, a.z / b.z});
}

bool	are_vectors_parallel(t_vector v1, t_vector v2)
{
	double	dot;

	v1 = vec_normalize(v1);
	v2 = vec_normalize(v2);
	dot = vec_dot(v1, v2);
	return ((fabs(dot - 1.0) < EPSILON) || (fabs(dot + 1.0) < EPSILON));
}
