/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range_search.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/26 22:51:53 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/26 22:52:30 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlxlib.h"

int ft_range_search(t_button *button, t_point *mouse)
{
	if (button->position.x <= mouse->x && mouse->x <= (button->position.x + button->width))
		if (button->position.y <= mouse->y && mouse->y <= (button->position.y + button->height))
			return (1);
	return (0);
}
