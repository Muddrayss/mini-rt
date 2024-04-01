/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 15:35:08 by craimond          #+#    #+#             */
/*   Updated: 2024/04/01 17:05:27 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/minirt.h"

static void		fill_octree(t_octree *node, t_list *shapes, uint8_t depth, t_vector box_top, t_vector box_bottom);
static void		set_world_extremes(t_scene *scene);
static void		set_bb_sphere(t_shape *shape);
static void		set_bb_cylinder(t_shape *shape);
static void		set_bb_plane(t_shape *shape);
static t_list	*get_shapes_inside_box(t_list *shapes, t_vector box_top, t_vector box_bottom);
static void 	set_shapes_data(t_scene *scene);

int  boxes_overlap(t_point box1_top, t_point box1_bottom, t_point box2_top, t_point box2_bottom)
{
    return (box1_bottom.x <= box2_top.x && box1_top.x >= box2_bottom.x &&
            box1_bottom.y <= box2_top.y && box1_top.y >= box2_bottom.y &&
            box1_bottom.z <= box2_top.z && box1_top.z >= box2_bottom.z);
}

void setup_scene(t_scene *scene)
{
	set_shapes_data(scene);
	set_world_extremes(scene);
	fill_octree(scene->octree, scene->shapes, OCTREE_DEPTH, scene->world_max, scene->world_min);
}

static void set_shapes_data(t_scene *scene)
{
	t_shape		*shape;
	t_list		*node;
	t_cylinder	*cylinder;
	
	node = scene->shapes;
	while (node)
	{
		shape = (t_shape *)node->content;
		set_bounding_box(shape);
		switch (shape->type)
		{
			case PLANE:
				shape->plane.normal = vec_normalize(shape->plane.normal);
				break ;
			case CYLINDER:
				cylinder = &shape->cylinder;
				cylinder->sqr_radius = cylinder->radius * cylinder->radius;
				cylinder->direction = vec_normalize(cylinder->direction);
				cylinder->top_cap_center = vec_add(cylinder->center, vec_scale(cylinder->direction, cylinder->height / 2.0));
				cylinder->bottom_cap_center = vec_sub(cylinder->center, vec_scale(cylinder->direction, cylinder->height / 2.0));
				break ;
			default:
				break ;
		}
		node = node->next;
	}
	scene->camera.normal = vec_normalize(scene->camera.normal);
}

static void fill_octree(t_octree *node, t_list *shapes, uint8_t depth, t_vector box_top, t_vector box_bottom)
{
	t_point		center;
	t_vector	size;
	t_point		new_box_top;
	t_point		new_box_bottom;
	t_list		*shapes_inside_box;
	uint16_t	i;

	if (depth == 0) //foglia
	{
		node->children = NULL; //fondamentale, e' cio' che distingue un nodo foglia da uno interno
		node->shapes = shapes;
		node->box_top = box_top;
		node->box_bottom = box_bottom;
		node->n_shapes = ft_lstsize(shapes);
		return ;
	}
	node->children = (t_octree **)malloc(sizeof(t_octree *) * 8);
	node->box_bottom = box_bottom;
	node->box_top = box_top;
	node->shapes = shapes;
	//world.min a sx e world.max a dx
	center = (t_vector){(box_top.x + box_bottom.x) / 2, (box_top.y + box_bottom.y) / 2, (box_top.z + box_bottom.z) / 2};
	size = (t_vector){(box_top.x - box_bottom.x) / 2, (box_top.y - box_bottom.y) / 2, (box_top.z - box_bottom.z) / 2};
	i = -1;
	while (++i < 8)
	{
		new_box_top = center;
		new_box_bottom = center;

		new_box_top.x += (i & 1) ? size.x : 0;
		new_box_bottom.x += (i & 1) ? 0 : -size.x;
		new_box_top.y += (i & 2) ? size.y : 0;
		new_box_bottom.y += (i & 2) ? 0 : -size.y;
		new_box_top.z += (i & 4) ? size.z : 0;
		new_box_bottom.z += (i & 4) ? 0 : -size.z;

		shapes_inside_box = get_shapes_inside_box(shapes, new_box_top, new_box_bottom);
		if (!shapes_inside_box)
		{
			node->children[i] = NULL;
			continue ;	
		}
		node->children[i] = (t_octree *)malloc(sizeof(t_octree));
		node->n_shapes = ft_lstsize(shapes_inside_box);
		fill_octree(node->children[i], shapes_inside_box, depth - 1, new_box_top, new_box_bottom);
	}
}

static t_list *get_shapes_inside_box(t_list *shapes, t_point box_top, t_point box_bottom)
{
    t_list  *inside_shapes = NULL;
	t_shape *current_shape;

    while (shapes)
    {
        current_shape = (t_shape*)shapes->content;
        if (boxes_overlap(box_top, box_bottom, current_shape->bb_max, current_shape->bb_min))
			ft_lstadd_front(&inside_shapes, ft_lstnew(current_shape));
        shapes = shapes->next;
    }
    return (inside_shapes);
}

static void	set_world_extremes(t_scene *scene)
{
	t_list		*shapes;
	t_shape		*shape;
	t_point		world_max = {0, 0, 0};
	t_point		world_min = {0, 0, 0};

	//bb min e bb max delle shapes deve partire da 0,0,0

	shapes = scene->shapes;
	if (!shapes)
		return ;
	shape = (t_shape *)shapes->content;
	while (shapes)
	{
		shape = shapes->content;
		world_max = (t_point){fmax(world_max.x, shape->bb_max.x), fmax(world_max.y, shape->bb_max.y), fmax(world_max.z, shape->bb_max.z)};
		world_min = (t_point){fmin(world_min.x, shape->bb_min.x), fmin(world_min.y, shape->bb_min.y), fmin(world_min.z, shape->bb_min.z)};
		shapes = shapes->next;
	}
	scene->world_min = world_min;
	scene->world_max = world_max;
}

void	set_bounding_box(t_shape *shape)
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

//bounding box for plane
static void set_bb_plane(t_shape *shape)
{
	t_vector 			u;
	t_vector 			v;
	t_vector 			r;
	static const float	size = WORLD_SIZE / 2;
	static const float  thickness = EPSILON; // Define a small thickness
	
	r = (t_vector){1, 0, 0};
	if (fabs(vec_dot(r, shape->plane.normal)) > 0.999)
    	r = (t_vector){0, 1, 0};
	
	u = vec_cross(r, shape->plane.normal);
	v = vec_cross(u, shape->plane.normal);

	u = vec_normalize(u);
	v = vec_normalize(v);

	t_point bb_min = {
    	-(size * fabs(u.x)) - (size * fabs(v.x)),
    	-(size * fabs(u.y)) - (size * fabs(v.y)),
    	-(size * fabs(u.z)) - (size * fabs(v.z))
	};

	t_point bb_max = {
		size * fabs(u.x) + (size * fabs(v.x)),
	 	size * fabs(u.y) + (size * fabs(v.y)),
		size * fabs(u.z) + (size * fabs(v.z))
	};

	// Add a small thickness to the bounding box
	bb_min.x -= shape->plane.normal.x * thickness;
	bb_min.y -= shape->plane.normal.y * thickness;
	bb_min.z -= shape->plane.normal.z * thickness;

	bb_max.x += shape->plane.normal.x * thickness;
	bb_max.y += shape->plane.normal.y * thickness;
	bb_max.z += shape->plane.normal.z * thickness;

	shape->bb_max = bb_max;
	shape->bb_min = bb_min;
}

//axis aligned bounding box for cylinder
static void	set_bb_cylinder(t_shape *shape)
{
	const t_vector	orientation = shape->cylinder.direction;
	const t_vector	center = shape->cylinder.center;

	const float r = shape->cylinder.radius;
	const float h = shape->cylinder.height;

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

	t_point bb_min = (t_point) axis_min;
	t_point bb_max = (t_point) axis_max;

   	bb_min.x -= r;
	bb_min.y -= r;
	bb_min.z -= r;

	bb_max.x += r;
	bb_max.y += r;
	bb_max.z += r;

	shape->bb_min = bb_min;
	shape->bb_max = bb_max;
}

