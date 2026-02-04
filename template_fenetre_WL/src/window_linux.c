/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_linux.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04                                 #+#    #+#           */
/*   Updated: 2026/02/04                                 #+#    #+#           */
/*                                                                            */
/* ************************************************************************** */

#ifndef _WIN32

# include "window.h"
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <stdlib.h>

/*
 * * Backend X11 stocké dans w->backend_*
 ** backend_1 = Display*
 ** backend_2 = Window (casté en void*)
 ** backend_3 = GC (Graphic Context)
 */

int	window_init(t_window *w, const char *title, int width, int height)
{
    Display	*disp;
    int		screen;
    Window	win;
    GC		gc;
    
    w->running = 1;
    w->title = title;
    w->width = width;
    w->height = height;
    
    disp = XOpenDisplay(NULL);
    if (!disp)
        return (1);
    screen = DefaultScreen(disp);
    
    win = XCreateSimpleWindow(disp, RootWindow(disp, screen),
                              0, 0, width, height, 1,
                              BlackPixel(disp, screen), WhitePixel(disp, screen));
    
    XStoreName(disp, win, title);
    
    /*
     * * On s’abonne aux événements importants :
     ** - Expose : la fenêtre doit être redessinée
     ** - KeyPress : touches (optionnel)
     ** - StructureNotify : pour Close (DestroyNotify)
     */
    XSelectInput(disp, win, ExposureMask | KeyPressMask | StructureNotifyMask);
    
    XMapWindow(disp, win);
    
    gc = XCreateGC(disp, win, 0, NULL);
    
    w->backend_1 = (void *)disp;
    w->backend_2 = (void *)(unsigned long)win;
    w->backend_3 = (void *)gc;
    return (0);
}

void	window_poll_events(t_window *w)
{
    Display	*disp;
    Window	win;
    XEvent	ev;
    
    disp = (Display *)w->backend_1;
    win = (Window)(unsigned long)w->backend_2;
    
    while (XPending(disp) > 0)
    {
        XNextEvent(disp, &ev);
        if (ev.type == DestroyNotify)
            w->running = 0;
        /*
         * * Exemple : si tu veux fermer avec une touche.
         ** ev.type == KeyPress -> lire la touche ici
         */
    }
    (void)win;
}

void	window_present(t_window *w)
{
    Display	*disp;
    Window	win;
    GC		gc;
    
    disp = (Display *)w->backend_1;
    win = (Window)(unsigned long)w->backend_2;
    gc = (GC)w->backend_3;
    
    /*
     * * Clear simple : on dessine un rectangle plein blanc.
     ** (Tu remplaceras ça par ton rendu plus tard)
     */
    XSetForeground(disp, gc, WhitePixel(disp, DefaultScreen(disp)));
    XFillRectangle(disp, win, gc, 0, 0, w->width, w->height);
    
    /*
     * * Exemple : dessiner un rectangle noir.
     */
    XSetForeground(disp, gc, BlackPixel(disp, DefaultScreen(disp)));
    XFillRectangle(disp, win, gc, 50, 50, 200, 120);
    
    XFlush(disp);
}

void	window_destroy(t_window *w)
{
    Display	*disp;
    Window	win;
    GC		gc;
    
    if (!w)
        return ;
    disp = (Display *)w->backend_1;
    win = (Window)(unsigned long)w->backend_2;
    gc = (GC)w->backend_3;
    if (disp)
    {
        if (gc)
            XFreeGC(disp, gc);
        if (win)
            XDestroyWindow(disp, win);
        XCloseDisplay(disp);
    }
    w->backend_1 = NULL;
    w->backend_2 = NULL;
    w->backend_3 = NULL;
}

#endif
