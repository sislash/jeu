/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04                                 #+#    #+#           */
/*   Updated: 2026/02/04                                 #+#    #+#           */
/*                                                                            */
/* ************************************************************************** */

#include "menu.h"

static int	ft_clamp(int v, int min, int max)
{
    if (v < min)
        return (min);
    if (v > max)
        return (max);
    return (v);
}

void	menu_init(t_menu *m, const char **items, int count)
{
    m->items = items;
    m->count = count;
    m->selected = 0;
}

int	menu_update(t_menu *m, t_window *w)
{
    if (w->key_up)
        m->selected--;
    if (w->key_down)
        m->selected++;
    m->selected = ft_clamp(m->selected, 0, m->count - 1);
    if (w->key_enter)
        return (m->selected);
    if (w->key_escape)
        return (m->count - 1);
    return (-1);
}

void	menu_render(t_menu *m, t_window *w, int x, int y)
{
    int	i;
    int	item_h;
    int	pad_x;
    int	pad_y;
    
    i = 0;
    item_h = 36;
    pad_x = 12;
    pad_y = 8;
    while (i < m->count)
    {
        if (i == m->selected)
            window_fill_rect(w, x, y + i * item_h,
                             260, item_h - 4, 0x111111);
        else
            window_fill_rect(w, x, y + i * item_h,
                            260, item_h - 4, 0xDDDDDD);
        if (i == m->selected)
            window_draw_text(w, x + pad_x, y + i * item_h + pad_y,
                            m->items[i], 0xFFFFFF);
        else
            window_draw_text(w, x + pad_x, y + i * item_h + pad_y,
                            m->items[i], 0x000000);
    i++;
    }
}
