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
# include <X11/keysym.h>
# include <stdlib.h>
# include <string.h>

static unsigned long	ft_x11_color(Display *d, int screen, int rgb)
{
    XColor		c;
    Colormap	cm;
    
    cm = DefaultColormap(d, screen);
    c.red = (unsigned short)(((rgb >> 16) & 0xFF) * 257);
    c.green = (unsigned short)(((rgb >> 8) & 0xFF) * 257);
    c.blue = (unsigned short)((rgb & 0xFF) * 257);
    c.flags = DoRed | DoGreen | DoBlue;
    if (XAllocColor(d, cm, &c) == 0)
        return (BlackPixel(d, screen));
    return (c.pixel);
}

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
    w->key_up = 0;
    w->key_down = 0;
    w->key_enter = 0;
    w->key_escape = 0;
    disp = XOpenDisplay(NULL);
    if (!disp)
        return (1);
    screen = DefaultScreen(disp);
    win = XCreateSimpleWindow(disp, RootWindow(disp, screen),
                              0, 0, width, height, 1,
                              BlackPixel(disp, screen), WhitePixel(disp, screen));
    XStoreName(disp, win, title);
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
    XEvent	ev;
    KeySym	ks;
    
    w->key_up = 0;
    w->key_down = 0;
    w->key_enter = 0;
    w->key_escape = 0;
    disp = (Display *)w->backend_1;
    while (disp && XPending(disp) > 0)
    {
        XNextEvent(disp, &ev);
        if (ev.type == DestroyNotify)
            w->running = 0;
        if (ev.type == KeyPress)
        {
            ks = XLookupKeysym(&ev.xkey, 0);
            if (ks == XK_Up)
                w->key_up = 1;
            else if (ks == XK_Down)
                w->key_down = 1;
            else if (ks == XK_Return || ks == XK_KP_Enter)
                w->key_enter = 1;
            else if (ks == XK_Escape)
                w->key_escape = 1;
        }
    }
}

void	window_clear(t_window *w, int color)
{
    Display			*disp;
    Window			win;
    GC				gc;
    int				screen;
    unsigned long	pix;
    
    disp = (Display *)w->backend_1;
    win = (Window)(unsigned long)w->backend_2;
    gc = (GC)w->backend_3;
    if (!disp || !gc)
        return ;
    screen = DefaultScreen(disp);
    pix = ft_x11_color(disp, screen, color);
    XSetForeground(disp, gc, pix);
    XFillRectangle(disp, win, gc, 0, 0, (unsigned int)w->width,
                   (unsigned int)w->height);
}

void	window_fill_rect(t_window *w, int x, int y, int width, int height,
                         int color)
{
    Display			*disp;
    Window			win;
    GC				gc;
    int				screen;
    unsigned long	pix;
    
    if (width <= 0 || height <= 0)
        return ;
    disp = (Display *)w->backend_1;
    win = (Window)(unsigned long)w->backend_2;
    gc = (GC)w->backend_3;
    if (!disp || !gc)
        return ;
    screen = DefaultScreen(disp);
    pix = ft_x11_color(disp, screen, color);
    XSetForeground(disp, gc, pix);
    XFillRectangle(disp, win, gc, x, y, (unsigned int)width,
                   (unsigned int)height);
}

void	window_draw_text(t_window *w, int x, int y, const char *text, int color)
{
    Display			*disp;
    Window			win;
    GC				gc;
    int				screen;
    unsigned long	pix;
    
    if (!text)
        return ;
    disp = (Display *)w->backend_1;
    win = (Window)(unsigned long)w->backend_2;
    gc = (GC)w->backend_3;
    if (!disp || !gc)
        return ;
    screen = DefaultScreen(disp);
    pix = ft_x11_color(disp, screen, color);
    XSetForeground(disp, gc, pix);
    XDrawString(disp, win, gc, x, y + 16, text, (int)strlen(text));
}

void	window_present(t_window *w)
{
    Display	*disp;
    
    disp = (Display *)w->backend_1;
    if (disp)
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
