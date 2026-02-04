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

static COLORREF	ft_win_color(int rgb)
{
    int	r;
    int	g;
    int	b;
    
    r = (rgb >> 16) & 0xFF;
    g = (rgb >> 8) & 0xFF;
    b = rgb & 0xFF;
    return (RGB(r, g, b));
}

static void	ft_set_key(t_window *w, WPARAM key)
{
    if (key == VK_UP)
        w->key_up = 1;
    else if (key == VK_DOWN)
        w->key_down = 1;
    else if (key == VK_RETURN)
        w->key_enter = 1;
    else if (key == VK_ESCAPE)
        w->key_escape = 1;
}

static LRESULT CALLBACK	ft_wndproc(HWND hwnd, UINT msg,
                                   WPARAM wparam, LPARAM lparam)
{
    t_window	*w;
    
    w = (t_window *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (msg == WM_CREATE)
    {
        w = (t_window *)((CREATESTRUCT *)lparam)->lpCreateParams;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)w);
        return (0);
    }
    if (msg == WM_KEYDOWN && w)
    {
        ft_set_key(w, wparam);
        return (0);
    }
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
    w->key_up = 0;
    w->key_down = 0;
    w->key_enter = 0;
    w->key_escape = 0;
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
                              NULL, NULL, b->hinst, w);
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
    
    w->key_up = 0;
    w->key_down = 0;
    w->key_enter = 0;
    w->key_escape = 0;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            w->running = 0;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void	window_clear(t_window *w, int color)
{
    t_win_backend	*b;
    HDC				hdc;
    RECT			rc;
    HBRUSH			brush;
    
    b = (t_win_backend *)w->backend_1;
    if (!b || !b->hwnd)
        return ;
    hdc = GetDC(b->hwnd);
    GetClientRect(b->hwnd, &rc);
    brush = CreateSolidBrush(ft_win_color(color));
    FillRect(hdc, &rc, brush);
    DeleteObject(brush);
    ReleaseDC(b->hwnd, hdc);
}

void	window_fill_rect(t_window *w, int x, int y, int width, int height,
                         int color)
{
    t_win_backend	*b;
    HDC				hdc;
    RECT			rc;
    HBRUSH			brush;
    
    if (width <= 0 || height <= 0)
        return ;
    b = (t_win_backend *)w->backend_1;
    if (!b || !b->hwnd)
        return ;
    hdc = GetDC(b->hwnd);
    rc.left = x;
    rc.top = y;
    rc.right = x + width;
    rc.bottom = y + height;
    brush = CreateSolidBrush(ft_win_color(color));
    FillRect(hdc, &rc, brush);
    DeleteObject(brush);
    ReleaseDC(b->hwnd, hdc);
}

void	window_draw_text(t_window *w, int x, int y, const char *text, int color)
{
    t_win_backend	*b;
    HDC				hdc;
    
    if (!text)
        return ;
    b = (t_win_backend *)w->backend_1;
    if (!b || !b->hwnd)
        return ;
    hdc = GetDC(b->hwnd);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, ft_win_color(color));
    TextOutA(hdc, x, y, text, (int)lstrlenA(text));
    ReleaseDC(b->hwnd, hdc);
}

void	window_present(t_window *w)
{
    (void)w;
}

void	window_destroy(t_window *w)
{
    t_win_backend	*b;
    
    b = (t_win_backend *)w->backend_1;
    if (b)
    {
        if (b->hwnd)
            DestroyWindow(b->hwnd);
        UnregisterClassA(b->class_name, b->hinst);
        free(b);
    }
    w->backend_1 = NULL;
    w->backend_2 = NULL;
    w->backend_3 = NULL;
}

#endif
