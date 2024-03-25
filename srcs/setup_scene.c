/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:35:08 by craimond          #+#    #+#             */
/*   Updated: 2024/03/25 19:18:38 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

static void 	fill_octree(t_octree *node, t_list *shapes, int depth, t_vector box_top, t_vector box_bottom);
static void		set_world_extremes(t_scene *scene);
static void		set_bb_sphere(t_shape *shape);
static void		set_bb_cylinder(t_shape *shape);
static void		set_bb_plane(t_shape *shape);
static t_list	*get_shapes_inside_box(t_list *shapes, t_vector box_top, t_vector box_bottom);

void setup_scene(t_scene *scene)
{
	set_world_extremes(scene);
	fill_octree(scene->octree, scene->shapes, 4, scene->world_max, scene->world_min);
}

static void fill_octree(t_octree *node, t_list *shapes, int depth, t_vector box_top, t_vector box_bottom)
{
	t_vector	center;
	t_vector	size;
	t_vector	new_box_top;
	t_vector	new_box_bottom;
	t_list		*shapes_inside_box;
	uint16_t	i;

	if (depth == 0)
	{
		node->shapes = shapes;
		node->box_top = box_top;
		node->box_bottom = box_bottom;
		node->n_shapes = ft_lstsize(shapes);
		return ;
	}
	//world.min a sx e world.max a dx
	center = (t_vector){(box_top.x + box_bottom.x) / 2, (box_top.y + box_bottom.y) / 2, (box_top.z + box_bottom.z) / 2};
	size = (t_vector){(box_top.x - box_bottom.x) / 2, (box_top.y - box_bottom.y) / 2, (box_top.z - box_bottom.z) / 2};
	i = 0;
	while (i < 8)
	{
		new_box_top = center;
		new_box_bottom = center;

		new_box_top.x += (i & 1) ? size.x : 0;
		new_box_bottom.x += (i & 1) ? 0 : -size.x;
		new_box_top.y += (i & 2) ? size.y : 0;
		new_box_bottom.y += (i & 2) ? 0 : -size.y;
		new_box_top.z += (i & 4) ? size.z : 0;
		new_box_bottom.z += (i & 4) ? 0 : -size.z;

		node->children[i] = (t_octree *)malloc(sizeof(t_octree));
		shapes_inside_box = get_shapes_inside_box(shapes, new_box_top, new_box_bottom);
		fill_octree(node->children[i], shapes_inside_box, depth - 1, new_box_top, new_box_bottom);
		//TODO eventuale free di filtered shapes
		i++;
	}
}

static t_list *get_shapes_inside_box(t_list *shapes, t_vector box_top, t_vector box_bottom)
{
	t_list		*shapes_inside_box;
	t_shape		*shape;
	t_vector	bb_min;
	t_vector	bb_max;

	shapes_inside_box = NULL;
	while (shapes)
	{
		shape = shapes->content;
		bb_min = shape->bb_min;
		bb_max = shape->bb_max;
		if (bb_min.x >= box_bottom.x && bb_max.x <= box_top.x &&
			bb_min.y >= box_bottom.y && bb_max.y <= box_top.y &&
			bb_min.z >= box_bottom.z && bb_max.z <= box_top.z)
			ft_lstadd_back(&shapes_inside_box, ft_lstnew(shape));
		shapes = shapes->next;
	}
	return (shapes_inside_box);
}

static void	set_world_extremes(t_scene *scene)
{
	t_list		*shapes;
	t_shape		*shape;
	t_vector	world_max = {0, 0, 0};
	t_vector	world_min = {0, 0, 0};

	//bb min e bb max delle shapes deve partire da 0,0,0

	shapes = scene->shapes;
	if (!shapes)
		return ;
	shape = (t_shape *)shapes->content;
	while (shapes)
	{
		shape = shapes->content;
		world_max = (t_vector){fmax(world_max.x, shape->bb_max.x), fmax(world_max.y, shape->bb_max.y), fmax(world_max.z, shape->bb_max.z)};
		world_min = (t_vector){fmin(world_min.x, shape->bb_min.x), fmin(world_min.y, shape->bb_min.y), fmin(world_min.z, shape->bb_min.z)};
		shapes = shapes->next;
	}
	scene->world_min = world_min;
	scene->world_max = world_max;
}

void set_bounding_box(t_shape *shape)
{
	void (*const	get_bb_funcs[])(t_shape *) = {&set_bb_sphere, &set_bb_cylinder, &set_bb_plane}; //deve essere lo stesso ordine dell enum type
	const uint8_t	n_shapes = sizeof(get_bb_funcs) / sizeof(get_bb_funcs[0]);
	uint8_t			i;

	i = 0;
	while (i < n_shapes)
	{
		if (shape->type == i)
		{
			get_bb_funcs[i](shape);
			return ;
		}
		i++;
	}
}

//bounding box for sphere
static void	set_bb_sphere(t_shape *shape)
{
	shape->bb_min = (t_point){shape->sphere.center.x - shape->sphere.radius, shape->sphere.center.y - shape->sphere.radius, shape->sphere.center.z - shape->sphere.radius};
	shape->bb_max = (t_point){shape->sphere.center.x + shape->sphere.radius, shape->sphere.center.y + shape->sphere.radius, shape->sphere.center.z + shape->sphere.radius};
}

static void set_bb_plane(t_shape *shape)
{
	shape->bb_min = (t_point){-WORLD_SIZE, -WORLD_SIZE, -WORLD_SIZE};
	shape->bb_max = (t_point){WORLD_SIZE, WORLD_SIZE, WORLD_SIZE};
}

//axis aligned bounding box for cylinder
static void	set_bb_cylinder(t_shape *shape)
{
	const t_vector	orientation = shape->cylinder.normal;
	const t_vector	center = shape->cylinder.center;

	const float r = shape->cylinder.diameter / 2;
	const float h = shape->cylinder.height;

	t_point bb_min;
	t_point bb_max;
	
	t_vector axis_max = (t_vector){
        center.x + orientation.x * (h / 2),
        center.y + orientation.y * (h / 2),
        center.z + orientation.z * (h / 2),
	};
	t_vector axis_min = (t_vector){
        center.x - orientation.x * (h / 2),
        center.y - orientation.y * (h / 2),
        center.z - orientation.z * (h / 2),
	};

	bb_min = axis_min;
    bb_max = axis_max;

    float expandX = sqrt(1 - orientation.x * orientation.x) * r;
    float expandY = sqrt(1 - orientation.y * orientation.y) * r;
    float expandZ = sqrt(1 - orientation.z * orientation.z) * r;

    bb_min.x = fmin(bb_min.x, center.x - expandX);
    bb_max.x = fmax(bb_max.x, center.x + expandX);

    bb_min.y = fmin(bb_min.y, center.y - expandY);
    bb_max.y = fmax(bb_max.y, center.y + expandY);

    bb_min.z = fmin(bb_min.z, center.z - expandZ);
    bb_max.z = fmax(bb_max.z, center.z + expandZ);

	shape->bb_min = bb_min;
	shape->bb_max = bb_max;
}

