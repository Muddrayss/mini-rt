/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 14:28:36 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:10:06 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minirt.h"

inline double	fclamp(const double value, const double min, const double max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

inline int32_t	clamp(const int32_t value, const int32_t min, const int32_t max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}
