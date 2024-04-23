/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 18:30:36 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 19:00:45 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

void	recurse_8_times(t_octree *node, t_point center, t_vector size, uint8_t depth, t_list *shapes)
{
	uint16_t	i;
	t_point		new_bt_bb[2];
	t_list		*shapes_inside_box;

	i = -1;
	while (++i < 8)
	{
		new_bt_bb[0] = center;
		new_bt_bb[1] = center;
		update_box_limits(&new_bt_bb[0], &new_bt_bb[1], size, i);
		shapes_inside_box = get_shapes_inside_box(shapes,
				new_bt_bb[0], new_bt_bb[1]);
		if (!shapes_inside_box)
		{
			node->children[i] = NULL;
			continue ;
		}
		node->children[i] = (t_octree *)calloc_p(1, sizeof(t_octree));
		node->n_shapes += ft_lstsize(shapes_inside_box);
		fill_octree(node->children[i],
			shapes_inside_box, depth - 1, new_bt_bb);
	}
}

void	fill_octree(t_octree *node, t_list *shapes,
	uint8_t depth, t_vector bt_bb[2])
{
	t_point		center;
	t_vector	size;

	if (depth == 0)
	{
		node->children = NULL;
		node->shapes = shapes;
		node->box_top = bt_bb[0];
		node->box_bottom = bt_bb[1];
		node->n_shapes = ft_lstsize(shapes);
		return ;
	}
	node->children = (t_octree **)calloc_p(8, sizeof(t_octree *));
	node->box_bottom = bt_bb[1];
	node->box_top = bt_bb[0];
	node->shapes = shapes;
	node->n_shapes = 0;
	center = (t_vector){(bt_bb[0].x + bt_bb[1].x) / 2,
		(bt_bb[0].y + bt_bb[1].y) / 2, (bt_bb[0].z + bt_bb[1].z) / 2};
	size = (t_vector){(bt_bb[0].x - bt_bb[1].x) / 2,
		(bt_bb[0].y - bt_bb[1].y) / 2, (bt_bb[0].z - bt_bb[1].z) / 2};
	recurse_8_times(node, center, size, depth, shapes);
}

t_list	*get_shapes_inside_box(t_list *shapes,
	t_point box_top, t_point box_bottom)
{
	t_list	*inside_shapes;
	t_shape	*current_shape;

	inside_shapes = NULL;
	while (shapes)
	{
		current_shape = (t_shape *)shapes->content;
		if (boxes_overlap(box_top, box_bottom,
				current_shape->bb_max, current_shape->bb_min))
			ft_lstadd_front(&inside_shapes, ft_lstnew(current_shape));
		shapes = shapes->next;
	}
	return (inside_shapes);
}
