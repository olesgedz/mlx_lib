/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 19:07:36 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/12 01:32:00 by olesgedz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void			ft_image_set_pixel(t_image *image, int x, int y, int color)
{
	if (y >= 0 && x >= 0 && y < WIN_HEIGHT && x < WIN_WIDTH)
		*(int *)(image->ptr + ((x + y * WIN_WIDTH) * image->bpp)) = color;
}

void			ft_image_set_pixel_tree(t_image *image, int x, int y, int color)
{
	if (y >= 0 && x >= 0 && y < FRAC_H && x < FRAC_W)
		*(int *)(image->ptr + ((x + y * WIN_WIDTH) * image->bpp)) = color;
}
