/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_background.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 21:24:24 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/26 21:25:39 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlxlib.h"

void			ft_draw_background(t_mlx *mlx)
{
	t_image *image;
	int		j;
	int		k;

	ft_clear_image(mlx->image);
	image = mlx->image;
	j = 0;
	while (j < WIN_HEIGHT)
	{
		k = 0;
		while (k++ < WIN_WIDTH)
			*(int *)(image->ptr + ((k + j * WIN_WIDTH) * image->bpp)) =
			k < FRAC_W ? 0x222222 : 0x1E1E1E;
		j++;
	}
}
