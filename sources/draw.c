/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:27:18 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/17 19:58:29 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "libft.h"
#include "mlx.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>


char *ft_strchnew(size_t n, char c)
{
	char *str;

	str = ft_memset(ft_strnew(n), c, n);
	return(str);
}
static int			ft_draw_menu(t_mlx *mlx)
{
	int		y;
	char *s;
	y = 0;
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 10, y, 0xFFFFFFF, "How to Use");
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 10, y += 25, 0xFFFFFFF, "UO to scale");
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 10, y += 25, 0xFFFFFFF, "WASD to shift");
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 10, y += 25, 0xFFFFFFF, "IJKL to rotate or ");
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 30, y += 25, 0xFFFFFFF, "press lmb and use mouse");
	mlx_string_put(mlx->mlx, mlx->window,
		 FRAC_H + 10, y += 25, 0xFFFFFFF, "M< to rotate on Z axis");
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 10, y += 25, 0xFFFFFFF, "B to change color");
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 10, y += 25, 0xFFFFFFF, ft_strjoin("Number of iterations: ", s = ft_itoa(mlx->n)));
	ft_strdel(&s);
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 10, y += 25, 0xFFFFFFF, ft_strjoin("scale: ", s = ft_itoa((int)mlx->cam->scale)));
	ft_strdel(&s);
	mlx_string_put(mlx->mlx, mlx->window,
		FRAC_H + 120, y += 35, 0xFFFFFFF, s = ft_strjoin(ft_strchnew(((18 - ft_strlen(mlx->fractal[mlx->nfractal].name)) / 2), ' '),
		 mlx->fractal[mlx->nfractal].name));
	ft_strdel(&s);
	return (0);
}

t_pixel	mandelbrot(t_mlx *e, int x, int y)
{
	int		i;
	double	za;
	double	zb;
	double	tmp;
	t_complex c;
	e->fractal[e->nfractal].ca = 1.5 * (x - FRAC_W / 2) / (0.5 * e->cam->scale * FRAC_W)
		+ (e->cam->offsetx / FRAC_W / 1.37) - 0.5;
	e->fractal[e->nfractal].cb = (y - FRAC_H / 2) / (0.5 * e->cam->scale * FRAC_H)
		- (e->cam->offsety / FRAC_H / 1.92);
	za = 0;
	zb = 0;
	i = 0;
	while (za * za + zb * zb <= 4 && i < e->n)
	{
		tmp = za;
		za = tmp * tmp - zb * zb + e->fractal[e->nfractal].ca;
		zb = 2 * tmp * zb + e->fractal[e->nfractal].cb;
		i++;
	}
	c.r = za;
	c.i = zb;
	return ((t_pixel){.c = c, .i = i});
}


static int			ft_put_points(t_mlx *mlx,
		t_line *l, t_point *p1, t_point *p2)
{
	double percentage;

	if (l->dx > l->dy)
		percentage = ft_percent(l->start.x, l->end.x, p1->x);
	else
		percentage = ft_percent(l->start.y, l->end.y, p1->y);
	ft_image_set_pixel_tree(mlx->image, (int)p1->x,
	(int)p1->y, ft_get_color(mlx->palette[mlx->ncolor].colors[0], mlx->palette[mlx->ncolor].colors[1], percentage));
	l->err2 = l->err;
	if (l->err2 > -l->dx)
	{
		l->err -= l->dy;
		p1->x += l->sx;
	}
	if (l->err2 < l->dy)
	{
		l->err += l->dx;
		p1->y += l->sy;
	}
	return (0);
}

static void			ft_plotline(t_mlx *mlx, t_point p1, t_point p2)
{
	t_line	line;

	p1.x = (int)p1.x;
	p2.x = (int)p2.x;
	p1.y = (int)p1.y;
	p2.y = (int)p2.y;
	line.start = p1;
	line.end = p2;
	line.dx = (int)ABS((int)p2.x - (int)p1.x);
	line.sx = (int)p1.x < (int)p2.x ? 1 : -1;
	line.dy = (int)ABS((int)p2.y - (int)p1.y);
	line.sy = (int)p1.y < (int)p2.y ? 1 : -1;
	line.err = (line.dx > line.dy ? line.dx : -line.dy) / 2;
	while (((int)p1.x != (int)p2.x || (int)p1.y != (int)p2.y))
		if (ft_put_points(mlx, &line, &p1, &p2))
			break ;
}


void		draw_tree(t_mlx *mlx, t_point start, double angle, int iter)
{
	t_point	end;

	if (iter > 0)
	{
		end.x = start.x + (cos(angle) * iter * 6) * mlx->cam->scale;
		end.y = start.y + (sin(angle) * iter * 9) * mlx->cam->scale;
		ft_plotline(mlx, start, end);
		draw_tree(mlx, end, angle - (M_PI / 8 * mlx->size_tree * 2), iter - 1);
		draw_tree(mlx, end, angle + (M_PI / 8 * mlx->size_tree2 * 2), iter - 1);
	}
}

t_pixel				randomf(t_mlx *e, int x, int y)
{
	double	za;
	double	zb;
	double	temp;
	int		i;
	t_complex c;
	za = ((4 * (float)x / FRAC_W - 2) / e->cam->scale) + ((e->cam->offsetx / FRAC_W));
	zb = ((-4 * (float)y / FRAC_W + 2) / e->cam->scale) + ((e->cam->offsety / FRAC_W));
	i = 0;
	while (za * za + zb * zb <= 4 && i < e->n)
	{
		temp = za;
		za = za * za - zb * zb + e->fractal[e->nfractal].ca;
		zb = sin(2 * temp * zb + e->fractal[e->nfractal].cb);
		i++;
	}
	c.r = za;
	c.i = zb;
	return ((t_pixel){.c = c, .i = i});
}

t_pixel	carpet(t_mlx *e, int x, int y)
{
	int		i;
	t_complex c;

	i = 0;
	e->cam->scale = (e->cam->scale >= 5) ? 1.6 : e->cam->scale;
	e->cam->scale = (e->cam->scale <= 0.6) ? 1.6 : e->cam->scale;
	x = (x) / e->cam->scale;
	y = (y) / e->cam->scale;
	x = ABS(x);
	y = ABS(y);
	c.r = 0;
	c.i = 0;
	while ((x > 0 || y > 0) && i < e->n)
	{
		if (x % 3 == 1 && y % 3 == 1)
			return ((t_pixel){.c = c, .i = 0});
		x /= 3;
		y /= 3;
		i++;
	}

	return ((t_pixel){.c = c, .i = 20});
}


t_pixel		burningship(t_mlx *e, int x, int y)
{
	int		i;
	double	za;
	double	zb;
	double	tmp;
	t_complex c;
	e->fractal[e->nfractal].ca = 1.5 * (x - FRAC_W / 2) / (0.5 * e->cam->scale * FRAC_W)
		+ (e->cam->offsetx / FRAC_W / 1.30) - 0.5;
	e->fractal[e->nfractal].cb = (y - FRAC_H / 2) / (0.5 * e->cam->scale * FRAC_H)
		- (e->cam->offsety / FRAC_H / 1.97);
	za = 0;
	zb = 0;
	i = 0;
	while (za * za + zb * zb <= 4 && i < e->n)
	{
		tmp = za;
		za = fabs(tmp * tmp) - zb * zb + e->fractal[e->nfractal].ca;
		zb = 2 * fabs(tmp * zb) + e->fractal[e->nfractal].cb;
		i++;
	}
	c.r = za;
	c.i = zb;
	return ((t_pixel){.c = c, .i = i});
}

t_pixel		julia(t_mlx *mlx, int x, int y)
{
	double	za;
	double	zb;
	double	temp;
	int		i;
	t_complex c;
	za = ((4 * (float)x / FRAC_W - 2) / mlx->cam->scale) + ((mlx->cam->offsetx / FRAC_W));
	zb = ((-4 * (float)y / FRAC_H + 2) / mlx->cam->scale) + ((mlx->cam->offsety / FRAC_H));
	i = 0;
	while (za * za + zb * zb <= 4 && i < mlx->n)
	{
		temp = za;
		za = za * za - zb * zb + mlx->fractal[mlx->nfractal].ca;
		zb = 2 * temp * zb + mlx->fractal[mlx->nfractal].cb;
		i++;
	}
	c.r = za;
	c.i = zb;
	return ((t_pixel){.c = c, .i = i});
}

void	put_pixel(t_image *e, int x, int y, int coloration)
{
	int		r;
	int		g;
	int		b;

	e->stride = 2400;
	e->bpp = 32;
	r = (coloration & 0xFF0000) >> 16;
	g = (coloration & 0xFF00) >> 8;
	b = (coloration & 0xFF);
	if (y >= 0 && x >= 0 && y < WIN_HEIGHT && x < WIN_WIDTH)
	{
		 e->ptr[(y * e->stride) + ((e->bpp / 8) * x) + 2] = r;
		 e->ptr[(y * e->stride) + ((e->bpp / 8) * x) + 1] = g;
		 e->ptr[(y * e->stride) + ((e->bpp / 8) * x)] = b;
	}
}


static void			ft_draw_background(t_mlx *mlx)
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

void	draw_fractal(t_mlx *mlx)
{
	int		x;
	int		y;
	double		i;
	t_pixel temp;
	y = 0;
	while (y < FRAC_H)
	{
		x = 0;
		while (x < FRAC_W)
		{
			if ((*(mlx->data + y * WIN_WIDTH + x)).i != mlx->n)
				ft_image_set_pixel(mlx->image, x, y, get_color(*(mlx->data + y * WIN_WIDTH + x), mlx));
			x++;
		}
		y++;
	}
}

void		*render_thread(void *m)
{
	t_thread	*t;
	int			x;
	int			y;
	t = (t_thread *)m;
	y = WIN_HEIGHT / THREADS * t->id;
	while (y < WIN_HEIGHT / THREADS * (t->id + 1))
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			*(t->mlx->data + y * WIN_WIDTH + x)  = t->mlx->fractal[t->mlx->nfractal].pixel(t->mlx, x, y);
			x++;
		}
		y++;
	}
	return (NULL);
}



void ft_multi_threading(t_mlx *mlx)
{
	int			i;
	t_render	*r;

	i = 0;
	r = &mlx->render;
	while (i < THREADS)
	{
		r->args[i].id = i;
		r->args[i].mlx = mlx;
		pthread_create(r->threads + i, NULL, render_thread, &(r->args[i]));
		i++;
	}
	i = 0;
	while (i < THREADS)
	{
		pthread_join(r->threads[i], NULL);
		i++;
	}
}

void	ft_draw_switch(t_mlx *mlx)
{
	ft_draw_background(mlx);
	if (mlx->nfractal != 5)
	{
		ft_multi_threading(mlx);
		draw_fractal(mlx);
	}
	else
	{
		t_point start;
		start.x = FRAC_W / 2 - (mlx->cam->offsetx * (mlx->cam->scale / 4.1));
		start.y = FRAC_H- 20 + (mlx->cam->offsety * (mlx->cam->scale  / 1.2));
		draw_tree(mlx, start, -(M_PI / 2), mlx->n / 5 + 1);
	}
}

// void ft_draw_right_arr(t_mlx *mlx, t_button *button)
// {
// 	int y;
// 	int x;
// 	int d;
//
// 	y = button->position.y;
// 	d =  button->width;
// 	while (y < button->position.y + button->height / 2)
// 	{
// 		x = button->position.x;
// 		while(x < button->position.x + button->width - d)
// 		{
// 			ft_image_set_pixel(mlx->image, x, y, button->color);
// 			x++;
// 		}
// 		d-=2;
// 		y++;
// 	}
// 	while (y < button->position.y + button->height)
// 	{
// 		x = button->position.x;
// 		while(x < button->position.x + button->width - d)
// 		{
// 			ft_image_set_pixel(mlx->image, x, y, button->color);
// 			x++;
// 		}
// 		d+=2;
// 		y++;
// 	}
// }


float sign (t_point *p1, t_point *p2, t_point *p3)
{
    return (p1->x - p3->x) * (p2->y - p3->y) - (p2->x - p3->x) * (p1->y - p3->y);
}

int  PointInTriangle (t_point *pt, t_point *v1, t_point *v2, t_point *v3)
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

void ft_draw_tr(t_mlx *mlx, t_button *button, t_triangle *triangle)
{
	int y;
	int x;

	y = button->position.y;
	while (y < button->position.y + button->height)
	{
		x = button->position.x;
		while(x < button->position.x + button->width)
		{
			if	(PointInTriangle(&((t_point){x, y}), &((t_point){button->position.x + triangle->p1.x, button->position.y + triangle->p1.y}),
			 &((t_point){button->position.x + triangle->p2.x, button->position.y + triangle->p2.y}),
			  &((t_point){button->position.x + triangle->p3.x, button->position.y  + triangle->p3.y})))
				ft_image_set_pixel(mlx->image, x, y, triangle->color);
			x++;
		}
		y++;
	}
}


void	ft_draw_onebutton(t_mlx *mlx, t_button *button)
{
	int i = 0;
	//printf("%d\n",button->figures->p2.x);
	while (i < 3)
	{
		ft_draw_tr(mlx, button, button->figures + i);
		i++;
	}
		//
}
void ft_draw_buttons(t_mlx *mlx)
{
	int i;

	i = 0;
	while(i < BUTTONS_N)
	{
		ft_draw_onebutton(mlx, mlx->buttons + i);
		//ft_draw_right_arr(mlx, mlx->buttons + i);
		i++;
	}

}


void				ft_render(t_mlx *mlx)
{
	int			x;
	int			y;
	t_vector	v;
	t_map		*map;
	map = mlx->map;
	mlx->image = ft_new_image(mlx);
	ft_draw_switch(mlx);
	ft_draw_buttons(mlx);
	mlx_put_image_to_window(mlx->mlx, mlx->window, mlx->image->image, 0, 0);
	ft_draw_menu(mlx);
	mlx_destroy_image(mlx->mlx, mlx->image->image);
}
