/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditionals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:32:50 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 15:56:59 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minirt.h"

inline bool	is_comment(const char *line)
{
	return (*line == '#');
}

inline bool	ft_isspace(const char c)
{
	return (ft_strchr(spaces, c) != NULL);
}

bool	is_empty_line(const char *line)
{
	while (*line)
	{
		if (!ft_isspace(*line))
			return (false);
		line++;
	}
	return (true);
}
