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

#include "window.h"
#include <unistd.h>

/*
 * * Ici, ton programme reste 100% portable :
 ** - window_init() ouvre la fenêtre
 ** - window_poll_events() gère fermeture / events
 ** - window_present() rafraîchit l'affichage
 **
 ** Tu peux ensuite ajouter ton "moteur" de jeu (update, draw, etc)
 */

static void	ft_game_update(void)
{
    /*
     * * Tu mettras ton code ici :
     ** - calcul positions
     ** - collisions
     ** - logique
     */
}

static void	ft_game_render(void)
{
    /*
     * * Sur un vrai moteur, tu dessinerais ici.
     ** Dans ce template minimal, le dessin est fait côté window_present().
     */
}

int	main(void)
{
    t_window	w;
    
    if (window_init(&w, "42 - Template Fenetre", 800, 600) != 0)
        return (1);
    
    while (w.running)
    {
        window_poll_events(&w);
        
        ft_game_update();
        ft_game_render();
        
        window_present(&w);
        
        /*
         * * Petite pause pour éviter de faire 100% CPU.
         ** 16000 µs ~ 60 FPS (approximatif).
         */
        sleep(16000);
    }
    window_destroy(&w);
    return (0);
}
