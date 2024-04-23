/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   singleton.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:47:54 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:11:49 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minirt.h"

t_scene	*get_scene(t_scene *_scene)
{
	static t_scene	*scene = NULL;

	if (_scene)
		scene = _scene;
	return (scene);
}
