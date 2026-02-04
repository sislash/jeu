/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: you <you@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04                                 #+#    #+#           */
/*   Updated: 2026/02/04                                 #+#    #+#           */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

/*
 * * Ce header décrit une interface "portable".
 ** main.c n’a pas besoin de savoir si on est sur Windows ou Linux.
 **
 ** Les fichiers:
 **  - window_win.c   -> implémentation WinAPI
 **  - window_linux.c -> implémentation X11
 */

typedef struct s_window
{
    /*
     * * running = 1 tant que la fenêtre est ouverte.
     ** Si l’utilisateur ferme la fenêtre, running passe à 0.
     */
    int			running;
    
    /*
     * * title, width, height : paramètres utiles (optionnel).
     */
    const char	*title;
    int			width;
    int			height;
    
    /*
     * * "backend" : pointeurs internes (cachés) vers les objets OS
     ** (HWND / Display* / etc).
     **
     ** On met des void* pour éviter d’inclure windows.h ou X11 ici.
     */
    void		*backend_1;
    void		*backend_2;
    void		*backend_3;
}	t_window;

/*
 * * Initialise et ouvre la fenêtre.
 ** Retourne 0 si ok, 1 si erreur.
 */
int		window_init(t_window *w, const char *title, int width, int height);

/*
 * * Poll des événements (fermeture, clavier, etc).
 ** Met w->running à 0 si la fenêtre doit se fermer.
 */
void	window_poll_events(t_window *w);

/*
 * * Présentation / rafraîchissement de la fenêtre.
 ** Dans ce template, on fait un "clear" simple.
 */
void	window_present(t_window *w);

/*
 * * Libère toutes les ressources OS.
 */
void	window_destroy(t_window *w);

#endif
