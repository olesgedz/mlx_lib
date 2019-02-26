/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_tr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 22:21:46 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/26 22:22:27 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "mlxlib.h"

float sign (t_point *p1, t_point *p2, t_point *p3)
{
    return (p1->x - p3->x) * (p2->y - p3->y) - (p2->x - p3->x) * (p1->y - p3->y);
}

int  ft_points_in_triangle(t_point *pt, t_point *v1, t_point *v2, t_point *v3)
{
    float d1, d2, d3;
    int has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void ft_draw_tr(t_mlx *mlx, t_button *button, t_figure *triangle)
{
	int y;
	int x;

	y = button->position.y;
	while (y < button->position.y + button->height)
	{
		x = button->position.x;
		while(x < button->position.x + button->width)
		{
			if	(ft_points_in_triangle(&((t_point){x, y}), &((t_point){button->position.x + triangle->p[0].x, button->position.y + triangle->p[0].y}),
			 &((t_point){button->position.x + triangle->p[1].x, button->position.y + triangle->p[1].y}),
			  &((t_point){button->position.x + triangle->p[2].x, button->position.y  + triangle->p[2].y})))
				ft_image_set_pixel(mlx->image, x, y, triangle->color);
			x++;
		}
		y++;
	}
}
