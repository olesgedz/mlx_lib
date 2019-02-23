/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jblack-b <jblack-b@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:24:27 by jblack-b          #+#    #+#             */
/*   Updated: 2019/02/16 21:06:44 by jblack-b         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include "mlx.h"
#include <stdlib.h>

void ft_zoom_to_mouse(t_mlx *mlx)
{
	mlx->cam->scale *= 1.1;
	mlx->cam->offsetx += (mlx->mouse->x - FRAC_W / 2)/ mlx->cam->scale / 2.51;
	mlx->cam->offsety -= (mlx->mouse->y - FRAC_H / 2) / mlx->cam->scale/ 2.51;
}

static void		ft_press_move(t_mlx *mlx)
{
	if (mlx->keyboard->keys[D_KEY] == TRUE)
		mlx->cam->offsetx += 20.0 / mlx->cam->scale;
	if (mlx->keyboard->keys[A_KEY] == TRUE)
		mlx->cam->offsetx -= 20.0 / mlx->cam->scale;
	if (mlx->keyboard->keys[S_KEY] == TRUE)
		mlx->cam->offsety -= 20 / mlx->cam->scale;
	if (mlx->keyboard->keys[W_KEY] == TRUE)
		mlx->cam->offsety += 20 / mlx->cam->scale;
	if (mlx->keyboard->keys[U_KEY] == TRUE)
		mlx->cam->scale /= 1.1;
	if (mlx->keyboard->keys[O_KEY] == TRUE)
		ft_zoom_to_mouse(mlx);
	if (mlx->keyboard->keys[M_KEY] == TRUE)
		mlx->cam->z -= 0.1;
	if (mlx->keyboard->keys[L_AR_KEY] == TRUE)
		mlx->cam->z += 0.1;
	if (mlx->keyboard->keys[I_KEY] == TRUE)
		mlx->n++;
	if (mlx->keyboard->keys[J_KEY] == TRUE)
		mlx->cam->y -= 0.1;
	if (mlx->keyboard->keys[K_KEY] == TRUE)
		mlx->n--;
	if (mlx->keyboard->keys[L_KEY] == TRUE)
		mlx->cam->y += 0.1;
}
void	ft_reset_view(t_mlx *mlx)
{
	mlx->cam->scale = 1;
	mlx->cam->offsetx = 0;
	mlx->cam->offsety = 0;
}

int				ft_handle_keys_press(int key, t_mlx *mlx)
{
	mlx->keyboard->keys[key] = TRUE;
	ft_press_move(mlx);
	if (key == B_KEY)
	{
		mlx->ncolor += 1;
		if (mlx->ncolor > 4)
			mlx->ncolor = 0;
	}
	if (key == F_KEY)
	{
		mlx->nfractal += 1;
		if (mlx->nfractal > FRACTAL_N - 1)
			mlx->nfractal = 0;
	}
	if (key == C_KEY)
		mlx->c =  (mlx->c == 0 ? 1 : 0);
	if (key == M_KEY)
		mlx->smooth = (mlx->smooth == 0 ? 1 : 0);
	if (key == R_KEY)
		ft_reset_view(mlx);
	if (key == ESC_KEY)
		exit(0);
	ft_render(mlx);
	return (0);
}

int				ft_handle_keys_release(int key, t_mlx *mlx)
{
	mlx->keyboard->keys[key] = FALSE;
	ft_render(mlx);
	return (0);
}

int			expose_hook(t_mlx *e)
{
	ft_render(e);
	return (0);
}

int ft_range_search(t_button *button, t_point *mouse)
{
	if (button->position.x <= mouse->x && mouse->x <= (button->position.x + button->width))
		if (button->position.y <= mouse->y && mouse->y <= (button->position.y + button->height))
			return (1);
	return (0);
}

void ft_button_is_pressed(t_mlx *mlx, t_button *button)
{
	if (mlx->mouse->isdown &&
		ft_range_search(button, &((t_point){mlx->mouse->x, mlx->mouse->y})))
	{
		button->pressed(mlx);
		ft_render(mlx);
	}
}

void	ft_handle_buttons(t_mlx *mlx)
{
	int i;

	i = 0;
	mlx->mouse->isdown = TRUE;
	while (i < BUTTONS_N)
		ft_button_is_pressed(mlx, &mlx->buttons[i++]);
}

int			mouse_hook(int button, int x, int y, t_mlx *mlx)
{
	mlx->mouse->x = x;
	mlx->mouse->y = y;
	if (button == SCROLL_UP)
		ft_zoom_to_mouse(mlx);
	else if (button == SCROLL_DOWN &&  mlx->cam->scale > 0.1)
		 mlx->cam->scale /= 1.1;
	if (button == MOUSE_L_KEY)
		ft_handle_buttons(mlx);
	if (button == SCROLL_UP || button == SCROLL_DOWN)
		ft_render(mlx);
	return (0);
}


int				ft_mlx_hooks(t_mlx *mlx)
{
	mlx_hook(mlx->window, 2, 0, ft_handle_keys_press, (void *)mlx);
	mlx_expose_hook(mlx->window, expose_hook, mlx);
	mlx_hook(mlx->window, 3, 0, ft_handle_keys_release, (void *)mlx);
	mlx_hook(mlx->window, 4, (1L << 2), mouse_hook, (void *)mlx);
	mlx_hook(mlx->window, 5, 0, ft_mouse_release, (void *)mlx);
	mlx_hook(mlx->window, 6, 0, ft_mouse_move, (void *)mlx);
	mlx_hook(mlx->window, 17, 0, ft_error, (void *)0);
	return (0);
}
