/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_freematrix.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egualand <egualand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:07:44 by craimond          #+#    #+#             */
/*   Updated: 2024/04/23 16:05:57 by egualand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minirt.h"

void	ft_freematrix(void **matrix)
{
	void	**tmp;

	tmp = matrix;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(matrix);
}
