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
# include <stdlib.h>

typedef struct s_win_backend
{
    HINSTANCE	hinst;
    HWND		hwnd;
    const char	*class_name;
}	t_win_backend;

static LRESULT CALLBACK	ft_wndproc(HWND hwnd, UINT msg,
                                   WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_CLOSE)
    {
        DestroyWindow(hwnd);
        return (0);
    }
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return (0);
    }
    return (DefWindowProc(hwnd, msg, wparam, lparam));
}

static int	ft_register_class(t_win_backend *b)
{
    WNDCLASSA	wc;
    DWORD		err;
    
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = ft_wndproc;
    wc.hInstance = b->hinst;
    wc.lpszClassName = b->class_name;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (RegisterClassA(&wc))
        return (0);
    err = GetLastError();
    if (err == ERROR_CLASS_ALREADY_EXISTS)
        return (0);
    return (1);
}

int	window_init(t_window *w, const char *title, int width, int height)
{
    t_win_backend	*b;
    RECT			r;
    
    w->running = 1;
    w->title = title;
    w->width = width;
    w->height = height;
    b = (t_win_backend *)malloc(sizeof(t_win_backend));
    if (!b)
        return (1);
    ZeroMemory(b, sizeof(*b));
    b->hinst = GetModuleHandleA(NULL);
    b->class_name = "ft_window_class";
    if (ft_register_class(b) != 0)
        return (free(b), 1);
    r.left = 0;
    r.top = 0;
    r.right = width;
    r.bottom = height;
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    b->hwnd = CreateWindowExA(0, b->class_name, title, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              r.right - r.left, r.bottom - r.top,
                              NULL, NULL, b->hinst, NULL);
    if (!b->hwnd)
        return (free(b), 1);
    ShowWindow(b->hwnd, SW_SHOW);
    UpdateWindow(b->hwnd);
    w->backend_1 = (void *)b;
    w->backend_2 = NULL;
    w->backend_3 = NULL;
    return (0);
}

void	window_poll_events(t_window *w)
{
    MSG	msg;
    
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
        if (b->class_name)
            UnregisterClassA(b->class_name, b->hinst);
        free(b);
    }
    w->backend_1 = NULL;
    w->backend_2 = NULL;
    w->backend_3 = NULL;
}

#endif
