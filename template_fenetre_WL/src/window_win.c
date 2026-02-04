/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window_win.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04                                 #+#    #+#           */
/*   Updated: 2026/02/04                                 #+#    #+#           */
/*                                                                            */
/* ************************************************************************** */

#ifdef _WIN32

# include "window.h"
# include <windows.h>

typedef struct s_win_backend
{
    HINSTANCE	hinst;
    HWND		hwnd;
}	t_win_backend;

/*
 * * Procédure de fenêtre : Windows envoie tous les messages ici
 ** (fermeture, resize, clavier, etc).
 */
static LRESULT CALLBACK ft_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return (0);
    }
    return (DefWindowProc(hwnd, msg, wparam, lparam));
}

int	window_init(t_window *w, const char *title, int width, int height)
{
    WNDCLASSA	wc;
    t_win_backend	*b;
    RECT		r;
    
    w->running = 1;
    w->title = title;
    w->width = width;
    w->height = height;
    
    b = (t_win_backend *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                                   sizeof(t_win_backend));
    if (!b)
        return (1);
    
    b->hinst = GetModuleHandleA(NULL);
    
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = ft_wndproc;
    wc.hInstance = b->hinst;
    wc.lpszClassName = "ft_window_class";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    if (!RegisterClassA(&wc))
    {
        HeapFree(GetProcessHeap(), 0, b);
        return (1);
    }
    
    /*
     * * Ajuster la taille pour que la zone CLIENT fasse width x height.
     */
    r.left = 0;
    r.top = 0;
    r.right = width;
    r.bottom = height;
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    
    b->hwnd = CreateWindowExA(
        0,
        "ft_window_class",
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        r.right - r.left, r.bottom - r.top,
        NULL, NULL, b->hinst, NULL);
    
    if (!b->hwnd)
    {
        HeapFree(GetProcessHeap(), 0, b);
        return (1);
    }
    
    ShowWindow(b->hwnd, SW_SHOW);
    UpdateWindow(b->hwnd);
    
    w->backend_1 = (void *)b;
    w->backend_2 = NULL;
    w->backend_3 = NULL;
    return (0);
}

void	window_poll_events(t_window *w)
{
    MSG		msg;
    
    /*
     * * PeekMessage : non-bloquant
     ** -> permet d’avoir une boucle de jeu fluide.
     */
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            w->running = 0;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void	window_present(t_window *w)
{
    t_win_backend	*b;
    HDC				hdc;
    RECT			rc;
    HBRUSH			brush_bg;
    HBRUSH			brush_rect;
    
    b = (t_win_backend *)w->backend_1;
    if (!b || !b->hwnd)
        return ;
    
    /*
     * * Rendu ultra simple avec GDI :
     ** - on remplit le fond
     ** - on dessine un rectangle
     **
     ** (Plus tard tu remplaceras par un rendu plus sérieux)
     */
    hdc = GetDC(b->hwnd);
    GetClientRect(b->hwnd, &rc);
    
    brush_bg = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rc, brush_bg);
    DeleteObject(brush_bg);
    
    brush_rect = CreateSolidBrush(RGB(0, 0, 0));
    rc.left = 50;
    rc.top = 50;
    rc.right = 250;
    rc.bottom = 170;
    FillRect(hdc, &rc, brush_rect);
    DeleteObject(brush_rect);
    
    ReleaseDC(b->hwnd, hdc);
}

void	window_destroy(t_window *w)
{
    t_win_backend	*b;
    
    b = (t_win_backend *)w->backend_1;
    if (b)
    {
        if (b->hwnd)
            DestroyWindow(b->hwnd);
        HeapFree(GetProcessHeap(), 0, b);
    }
    w->backend_1 = NULL;
    w->backend_2 = NULL;
    w->backend_3 = NULL;
}

#endif
