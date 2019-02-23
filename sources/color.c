/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:18:58 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/17 19:59:43 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <math.h>
static int		ft_get_light(int start, int end, double percentage)
{
	return ((int)((1 - percentage) * start + percentage * end));
}

double			ft_percent(int start, int end, int current)
{
	double placement;
	double distance;

	placement = current - start;
	distance = end - start;
	return ((distance == 0) ? 1.0 : (placement / distance));
}

int				ft_get_color(int c1, int c2, double p)
{
	int r;
	int g;
	int b;

	if (c1 == c2)
		return (c1);
	r = ft_get_light((c1 >> 16) & 0xFF, (c2 >> 16) & 0xFF, p);
	g = ft_get_light((c1 >> 8) & 0xFF, (c2 >> 8) & 0xFF, p);
	b = ft_get_light(c1 & 0xFF, c2 & 0xFF, p);
	return (r << 16 | g << 8 | b);
}

void			ft_fill_colors(t_map *m)
{
	t_vector	v;
	t_vector	*cur;

	v.y = 0;
	while (v.y < m->height)
	{
		v.x = 0;
		while (v.x < m->width)
		{
			cur = m->vectors[(int)v.y * m->width + (int)v.x];
			cur->color = ft_get_color(COLOR_BRICK_RED, 0xFFFFFF, (cur->z -
				m->depth_min) / (m->depth_max - m->depth_min));
			v.x++;
		}
		v.y++;
	}
}

t_color		clerp(t_color c1, t_color c2, double p)
{
	t_color c;

	if (c1.value == c2.value)
		return (c1);
	c.rgba.r = (char)ft_get_light((int)c1.rgba.r, (int)c2.rgba.r, p);
	c.rgba.g = (char)ft_get_light((int)c1.rgba.g, (int)c2.rgba.g, p);
	c.rgba.b = (char)ft_get_light((int)c1.rgba.b, (int)c2.rgba.b, p);
	c.rgba.a = (char)0x00;
	return (c);
}

t_color		linear_color(t_mlx *mlx, double i, t_palette *p)
{
	double		index = 0;
	double		adjust = 0;
	int			c = 0;

	if (p->cycle)
		index = fmod(i, p->cycle - 1) / (p->cycle - 1);
	else
		index = i / mlx->n;
	c = p->count - 1;
	adjust = fmod(index, 1.0f / c) * c;
	if (mlx->smooth)
		return (clerp((t_color)(p->colors[(int)(index * c) + 1]),
		(t_color)(p->colors[(int)(index * c)]),
		(int)(adjust + 1) - adjust));
	//return ((t_color)(p->colors[(int)(index * c)]));
	return (clerp((t_color)(p->colors[(int)(index * c) + 1]),
	(t_color)(p->colors[(int)(index * c)]),
	(int)(adjust + 1) - adjust));
}

t_color		smooth_color(t_mlx *mlx, t_pixel p, t_palette *pal)
{
	double i;
	double zn;
	double nu;

	 zn = log(p.c.r * p.c.r + p.c.i * p.c.i) / 2.0f;
	 nu = log(zn / log(2)) / log(2);
	i = p.i + 1 - nu;
	if (i < 0)
		i = 0;
	return(linear_color(mlx, i,  pal));
}

int			get_color(t_pixel p, t_mlx *mlx)
{
	if (p.i >= mlx->n)
		return(0x000000);
	if (mlx->smooth && mlx->nfractal != 4)
		return (smooth_color(mlx, p, mlx->palette + mlx->ncolor).value);
	return (linear_color(mlx, (double)p.i, mlx->palette + mlx->ncolor).value);
}
