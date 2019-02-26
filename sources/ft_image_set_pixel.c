/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_image_set_pixel.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 20:35:32 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/26 21:22:28 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlxlib.h"

void			ft_image_set_pixel(t_image *image, int x, int y, int color)
{
	if (y >= 0 && x >= 0 && y < WIN_HEIGHT && x < WIN_WIDTH)
		*(int *)(image->ptr + ((x + y * WIN_WIDTH) * image->bpp)) = color;
}
