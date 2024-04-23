/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:52:38 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:49:20 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

inline double	vec_length(const t_vector a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

inline t_vector	project_vector_onto_axis(t_vector vector, t_vector axis)
{
	t_vector	normalized_axis;
	double		dot_product;
	t_vector	projected_vec;

	normalized_axis = vec_normalize(axis);
	dot_product = vec_dot(vector, normalized_axis);
	projected_vec = vec_scale(dot_product, normalized_axis);
	return (projected_vec);
}
