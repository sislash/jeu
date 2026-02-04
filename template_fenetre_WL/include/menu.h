/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04                                 #+#    #+#           */
/*   Updated: 2026/02/04                                 #+#    #+#           */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

# include "window.h"

typedef struct s_menu
{
    const char	**items;
    int			count;
    int			selected;
}	t_menu;

void	menu_init(t_menu *m, const char **items, int count);
int		menu_update(t_menu *m, t_window *w);
void	menu_render(t_menu *m, t_window *w, int x, int y);

#endif
