/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 16:12:18 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/17 20:14:11 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FDF_H
# define FDF_H
# include <stdint.h>
# include <time.h>
# include <string.h>
#include <pthread.h>
#include "libft.h"
# define WIN_WIDTH		1680
# define WIN_HEIGHT		720
# define MENU_WIDTH		400
# define FRAC_W		720
# define FRAC_H		720
# define ESC_KEY	53
# define W_KEY 13
# define A_KEY 0
# define S_KEY 1
# define D_KEY 2
# define F_KEY 3
# define U_KEY 32
# define I_KEY 34
# define O_KEY 31
# define J_KEY 38
# define K_KEY 40
# define L_KEY 37
# define M_KEY 46
# define L_AR_KEY 43
# define B_KEY 11
# define R_KEY 15
# define TRUE		1
# define FALSE		0

# define SCROLL_UP		4
# define SCROLL_DOWN	5
# define MOUSE_L_KEY	1
# define MOUSE_R_KEY	2
# define MOUSE_S_KEY	3
# define C_KEY		8

# define TEXT_COLOR			0xEAEAEA
# define BACKGROUND			0x222222
# define MENU_BACKGROUND	0x1E1E1E

# define COLOR_DISCO		0x9A1F6A
# define COLOR_BRICK_RED	0xC2294E
# define COLOR_FLAMINGO		0xEC4B27
# define COLOR_JAFFA		0xEF8633
# define COLOR_SAFFRON		0xF3AF3D
# define FPS 200
# define THREADS 20
# define FRACTAL_N 6
# define BUTTONS_N 2
#define CHAR_HEIGHT 100
#define CHAR_WIDTH 100
typedef struct		s_line
{
	t_point	start;
	t_point	end;
	int			dx;
	int			dy;
	int			sx;
	int			sy;
	int			err;
	int			err2;
}					t_line;


typedef struct		s_palette
{
	uint8_t		count;
	int			cycle;
	int			colors[16];
}					t_palette;

typedef struct		s_cam
{
	double		offsetx;
	double		offsety;
	double		x;
	double		y;
	double		z;
	double			scale;
	double		**matrix;
}					t_cam;
typedef struct		s_vector
{
	double		x;
	double		y;
	double		z;
	int			color;
}					t_vector;
typedef struct		s_map
{
	int			width;
	int			height;
	int			depth_min;
	int			depth_max;
	t_vector	**vectors;
	double		*colors;
	int			ncolor;
}					t_map;
typedef struct		s_mouse
{
	char		isdown;
	int			x;
	int			y;
	int 	lastx;
	int 	lasty;
}					t_mouse;
typedef struct		s_image
{
	void		*image;
	char		*ptr;
	int			bpp;
	int			stride;
	int			endian;
}					t_image;
typedef struct		s_keyboard
{
	int		*keys;
}					t_keyboard;

typedef struct		s_rgba
{
	uint8_t		b;
	uint8_t		g;
	uint8_t		r;
	uint8_t		a;
}					t_rgba;

typedef struct		s_complex
{
	double		r;
	double		i;
}					t_complex;

typedef struct		s_pixel
{
	t_complex	c;
	double		i;
}					t_pixel;
typedef union		u_color
{
	int			value;
	t_rgba		rgba;
}					t_color;

typedef struct s_mlx	t_mlx;
typedef struct		s_thread
{
	int				id;
	t_mlx			*mlx; //fdsfs
}					t_thread;
typedef struct		s_render
{
	pthread_t		threads[THREADS];
	t_thread		args[THREADS];
}					t_render;

typedef t_pixel		(*t_func)(t_mlx *mlx, int x, int y);

typedef struct		s_fractal
{
	char		*name;
	t_func	pixel;
	double			ca;
	double			cb;
}					t_fractal;

typedef void	(*t_pressed)(t_mlx *mlx);

typedef struct s_triangle
{
	t_point p1;
	t_point p2;
	t_point p3;
	int color;
} t_triangle;

typedef struct s_button
{
	t_point position;
	int width;
	int height;
	t_pressed pressed;
	t_triangle *figures;
} t_button;


typedef struct		s_mlx
{
	void		*mlx;
	void		*window;
	t_image		*image;
	t_map		*map;
	t_cam		*cam;
	t_mouse		*mouse;
	t_keyboard	*keyboard;
	double		**zbuf;
	t_button    *buttons;
	clock_t clock_prg;
	int c;
	int n;
	int smooth;
	double *colors;
	t_color *color;
	int ncolor;
	t_palette *palette;
	t_pixel pixel;
	t_render	render;
	t_pixel *data;
	t_fractal *fractal;
	int nfractal;
	float size_tree;
	float size_tree2;
}					t_mlx;

int					ft_error(char *reason);
void				ft_change_color(t_mlx *mlx, int ncolor);
void				ft_render(t_mlx *mlx);
int					ft_mouse_press(int button, int x, int y, t_mlx *mlx);
int					ft_mouse_release(int button, int x, int y, t_mlx *mlx);
int					ft_mouse_move(int x, int y, t_mlx *mlx);
double				ft_percent(int start, int end, int current);
int					ft_get_color(int c1, int c2, double p);
void				ft_clear_image(t_image *image);
t_vector			ft_project_vector(t_vector v, t_mlx *mlx);
t_vector			ft_vector_at(t_map *map, int x, int y);
int					ft_read_file(int fd, t_map **m);
t_mlx				*ft_init(char *title);
int					ft_mlx_hooks(t_mlx *mlx);
t_vector			*ft_get_vector(int x, int y, int z);
void				ft_fill_colors(t_map *m);
t_map				*get_map(int width, int height);
t_image				*ft_del_image(t_mlx *mlx, t_image *img);
t_mlx				*ft_mlxdel(t_mlx *mlx);
void				ft_image_set_pixel(t_image *image, int x, int y, int color);
int					ft_check_line(char *s);
t_pixel				julia(t_mlx *mlx, int x, int y);
t_pixel				carpet(t_mlx *e, int x, int y);
t_pixel				mandelbrot(t_mlx *mlx, int x, int y);
t_pixel				burningship(t_mlx *mlx, int x, int y);
t_pixel				randomf(t_mlx *e, int x, int y);
void				draw_tree(t_mlx *mlx, t_point start, double angle, int iter);
void				draw_fractal(t_mlx *mlx);
t_image				*ft_new_image(t_mlx *mlx);
t_color 			*ft_colorHextoRgb(int hex);
int					get_color(t_pixel p, t_mlx *mlx);
void			ft_image_set_pixel_tree(t_image *image, int x, int y, int color);
#endif
