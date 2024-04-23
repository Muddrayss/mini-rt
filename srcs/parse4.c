/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:33:22 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:31:30 by egualand         ###   ########.fr       */
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

char	*skip_commas(char *str)
{
	while (*str && *str != ',')
		str++;
	if (*str == '\0')
		ft_quit(5, "invalid syntax: missing comma");
	return (str + 1);
}
