/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:18:29 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/17 20:12:18 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "libft.h"
#include "mlx.h"
#include "fractol.h"
#include <stdlib.h>
t_image		*ft_new_image(t_mlx *mlx)
{
	t_image		*img;

	if ((img = ft_memalloc(sizeof(t_image))) == NULL)
		return (NULL);
	if ((img->image = mlx_new_image(mlx->mlx, WIN_WIDTH, WIN_HEIGHT)) == NULL)
		return (ft_del_image(mlx, img));
	img->ptr = mlx_get_data_addr(img->image, &img->bpp, &img->stride,
			&img->endian);
	img->bpp /= 8;
	return (img);
}

t_palette	*get_palettes(void)
{
	static t_palette	array[6];

	array[0] =
		(t_palette){2, 0, {0xFF6666, 0xFFFF00}};
	array[1] =
		(t_palette){5, 0, {0x7F1637, 0x047878, 0xFFB733, 0xF57336, 0xC22121}};
	array[2] =
		(t_palette){5, 0, {0x0D1C33, 0x17373C, 0x2B6832, 0x4F9300, 0xA1D700}};
	array[3] =
		(t_palette){5, 0, {0x002F2F, 0x046380, 0xEFECCA, 0xA7A37E, 0xE6E2AF}};
	array[4] =
		(t_palette){7, 10, {0xFF0000, 0xFFFF00, 0x00FF00, 0x00FFFF,
			0x0000FF, 0xFF00FF, 0xFF0000}};
	array[5] = (t_palette){0, 0, {0}};
	return (array);
}

t_map				*get_map(int width, int height)
{
	t_map	*map;

	map = ft_memalloc(sizeof(t_map));
	if (map == NULL)
		return (NULL);
	map->width = width;
	map->height = height;
	map->depth_min = 0;
	map->depth_max = 0;
	map->vectors = ft_memalloc(sizeof(t_vector *) * width * height);
	if (map->vectors == NULL)
	{
		ft_memdel((void **)&map);
		return (NULL);
	}
	return (map);
}

t_fractal	*ft_get_fractals(t_mlx *mlx)
{
	static t_fractal fractals[FRACTAL_N];

	//fractals = ft_memalloc(sizeof(t_fractal) * 3);
	fractals[0] = (t_fractal){"Julia", julia};
	fractals[1] = (t_fractal){"Mandelbrot", mandelbrot};
	fractals[2] = (t_fractal){"Burningship", burningship};
	fractals[3] = (t_fractal){"Random#1", randomf};
	fractals[4] = (t_fractal){"Sierpinski Carpet", carpet};
	fractals[5] = (t_fractal){"Fractal Tree", NULL};
	return (fractals);
}

void	ft_next_fractal(t_mlx *mlx)
{
	mlx->nfractal += 1;
	if (mlx->nfractal > FRACTAL_N - 1)
		mlx->nfractal = 0;
}

void	ft_previous_fractal(t_mlx *mlx)
{
	mlx->nfractal -= 1;
	if (mlx->nfractal < 0)
		mlx->nfractal = FRACTAL_N - 1;
}

t_triangle *ft_get_triangles(t_triangle *tr1, t_triangle *tr2, t_triangle *tr3)
{
	t_triangle *figures;

	figures = malloc(sizeof(t_triangle) * 3);
	figures[0] = *tr1;
	figures[1] = *tr2;
	figures[2] = *tr3;
	return (figures);
}


t_button *ft_get_buttons()
{
	static t_button buttons[BUTTONS_N];
	buttons[0] = (t_button){{1050, 227}, 50, 50, ft_next_fractal,
		ft_get_triangles(&((t_triangle ){{0, 0},{50, 25},{0, 50}, 0xFFFF00}),
		&((t_triangle ){{0, 10},{40, 25},{0, 40}, 0xFF6666}),
		&((t_triangle ){{0, 10},{25, 25},{0, 40}, MENU_BACKGROUND}))};
	buttons[1] = (t_button){{750, 227}, 50, 50, ft_previous_fractal,
		ft_get_triangles(&((t_triangle ){{50, 0},{0, 25},{50, 50}, 0xFFFF00}),
		&((t_triangle ){{50, 10},{10, 25},{50, 40}, 0xFF6666}),
		&((t_triangle ){{50, 10},{25, 25},{50, 40}, MENU_BACKGROUND}))};
	return (buttons);
}

t_mlx				*ft_init(char *title)
{
	t_mlx	*mlx;

	if ((mlx = ft_memalloc(sizeof(t_mlx))) == NULL)
		return (NULL);
	if ((mlx->mlx = mlx_init()) == NULL ||
		(mlx->window = mlx_new_window(mlx->mlx, WIN_WIDTH,
			WIN_HEIGHT, title)) == NULL ||
		(mlx->cam = ft_memalloc(sizeof(t_cam))) == NULL ||
		(mlx->mouse = ft_memalloc(sizeof(t_mouse))) == NULL ||
		(mlx->keyboard = ft_memalloc(sizeof(t_keyboard))) == NULL ||
		(mlx->keyboard->keys = ft_memalloc(sizeof(int) * 200)) == NULL ||
		(mlx->image = ft_new_image(mlx)) == NULL || (mlx->color = ft_memalloc(sizeof(t_color))) == NULL)
		return (ft_mlxdel(mlx));
	ft_bzero((char *)mlx->keyboard->keys, 100);
	mlx->cam->x = -M_PI / 6;
	mlx->cam->y = -M_PI / 6;
	mlx->cam->z = 0;
	mlx->cam->scale = 1;
	mlx->c = 1;
	mlx->n = 32;
	mlx->palette = get_palettes();
	mlx->data = ft_memalloc(sizeof(t_pixel) * WIN_WIDTH * WIN_HEIGHT);
	//mlx->pixel = ft_memalloc(sizeof(t_pixel));
	mlx->buttons = ft_get_buttons();
	mlx->smooth = 1;
	mlx->clock_prg = clock();
	mlx->cam->offsetx = 0;//(WIN_WIDTH - MENU_WIDTH) / 2;
	mlx->cam->offsety = 0;//WIN_HEIGHT / 2;
	mlx->fractal = ft_get_fractals(mlx);
	mlx->nfractal = 0;
	mlx->fractal[mlx->nfractal].ca = 0;
	mlx->fractal[mlx->nfractal].cb = 0;
	mlx->size_tree = 1;
	mlx->size_tree2 = 1;
	return (mlx);
}

t_vector			*ft_get_vector(int x, int y, int z)
{
	t_vector	*v;

	v = ft_memalloc(sizeof(t_vector));
	if (v == NULL)
		return (NULL);
	v->x = (double)x;
	v->y = (double)y;
	v->z = (double)z;
	v->color = 0xFFFFFF;
	return (v);
}
