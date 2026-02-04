/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04                                 #+#    #+#           */
/*   Updated: 2026/02/04                                 #+#    #+#           */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 199309L

#include "utils.h"

#ifndef _WIN32
# include <time.h>

void	ft_sleep_ms(int ms)
{
    struct timespec	ts;
    
    if (ms <= 0)
        return ;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, 0);
}
#else
# include <windows.h>

void	ft_sleep_ms(int ms)
{
    if (ms <= 0)
        return ;
    Sleep((DWORD)ms);
}
#endif
