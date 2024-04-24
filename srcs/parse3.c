/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:33:22 by craimond          #+#    #+#             */
/*   Updated: 2024/04/24 14:33:03 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

t_color	parse_color(char *str)
{
	t_color	color;

	if (!str)
		ft_quit(5, "invalid color syntax");
	color.r = ft_atoui(str);
	str = skip_commas(str);
	color.g = ft_atoui(str);
	str = skip_commas(str);
	color.b = ft_atoui(str);
	return (color);
}

t_float3	parse_coord(char *str)
{
	t_float3	coord;

	if (!str)
		ft_quit(5, "invalid coordinate syntax");
	coord.x = ft_atof(str);
	str = skip_commas(str);
	coord.y = ft_atof(str);
	str = skip_commas(str);
	coord.z = ft_atof(str);
	return (coord);
}

char	*skip_commas(char *str)
{
	while (*str && *str != ',')
		str++;
	if (*str == '\0')
		ft_quit(5, "invalid syntax: missing comma");
	return (str + 1);
}
