/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04                                 #+#    #+#           */
/*   Updated: 2026/02/04                                 #+#    #+#           */
/*                                                                            */
/* ************************************************************************** */

#include "menu.h"
#include "utils.h"

static void	render_centered_text(t_window *w, const char *text)
{
    int	x;
    int	y;
    
    x = 40;
    y = w->height / 2;
    window_draw_text(w, x, y, text, 0x000000);
    window_draw_text(w, x, y + 24,
                     "Appuie sur Echap pour revenir au menu.", 0x000000);
}

int	main(void)
{
    t_window		w;
    t_menu			menu;
    const char		*items[3];
    int				screen;
    int				action;
    
    items[0] = "Jouer";
    items[1] = "Options";
    items[2] = "Quitter";
    menu_init(&menu, items, 3);
    if (window_init(&w, "42 - Menu simple", 800, 600) != 0)
        return (1);
    screen = 0;
    while (w.running)
    {
        window_poll_events(&w);
        if (screen == 0)
        {
            action = menu_update(&menu, &w);
            if (action == 0)
                screen = 1;
            else if (action == 1)
                screen = 2;
            else if (action == 2)
                w.running = 0;
            window_clear(&w, 0xFFFFFF);
            window_draw_text(&w, 40, 40, "Menu", 0x000000);
            menu_render(&menu, &w, 40, 90);
        }
        else if (screen == 1)
        {
            if (w.key_escape)
                screen = 0;
            window_clear(&w, 0xFFFFFF);
            render_centered_text(&w, "Ecran JEU (placeholder)");
        }
        else if (screen == 2)
        {
            if (w.key_escape)
                screen = 0;
            window_clear(&w, 0xFFFFFF);
            render_centered_text(&w, "Ecran OPTIONS (placeholder)");
        }
        window_present(&w);
        ft_sleep_ms(16);
    }
    window_destroy(&w);
    return (0);
}
