/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clear_image.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 20:38:44 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/26 21:23:03 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlxlib.h"

void		ft_clear_image(t_image *image)
{
	ft_bzero(image->ptr, WIN_WIDTH * WIN_HEIGHT * image->bpp);
}
