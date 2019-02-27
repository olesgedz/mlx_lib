/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_cr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 22:18:24 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/27 16:37:35 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "mlxlib.h"

static int		ft_points_in_circle(t_point *pt,
		t_point *center, t_point *radius, t_point *v3)
{
	if (((sqrt(pow((pt->x - center->x), 2) +
		pow((pt->y - center->y), 2))) < radius->x))
		return (1);
	return (0);
}

void			ft_draw_cr(t_mlx *mlx, t_button *button, t_figure *circle)
{
	int y;
	int x;

	y = button->position.y;
	while (y < button->position.y + button->height)
	{
		x = button->position.x;
		while (x < button->position.x + button->width)
		{
			if (ft_points_in_circle(&((t_point){x, y}),
			&((t_point){button->position.x + circle->p[0].x,
			button->position.y + circle->p[0].y}),
			&((t_point){circle->p[1].x,
			button->position.y + circle->p[1].y}),
			&((t_point){button->position.x + circle->p[2].x,
			button->position.y + circle->p[2].y})))
				ft_image_set_pixel(mlx->image, x, y, circle->color);
			x++;
		}
		y++;
	}
}
