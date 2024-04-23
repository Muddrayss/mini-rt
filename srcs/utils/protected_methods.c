/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   protected_methods.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 18:38:50 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:11:41 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minirt.h"

void	*calloc_p(const size_t n_memb, const size_t size)
{
	void	*ptr;

	ptr = ft_calloc(n_memb, size);
	if (!ptr)
		ft_quit(1, "fatal: failed to allocate memory");
	return (ptr);
}

void	*malloc_p(const size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		ft_quit(1, "fatal: failed to allocate memory");
	return (ptr);
}
