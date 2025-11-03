#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "globals/globals.h"
#include "types/types.h"
#include "world/world.h"
#include "render/render.h"
#include "input/input.h"
#include "player/player.h"

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // affichage console wd
#endif
    srand((unsigned)time(NULL));  // rand

    // Créa plongeur
    Plongeur plongeur = {
        .points_de_vie = 100,
        .points_de_vie_max = 100,
        .niveau_oxygene = 100,
        .niveau_oxygene_max = 100,
        .niveau_fatigue = 0,
        .niveau_fatigue_max = 100,
        .perles = 10,
        .pos_y      = 5,
        .pos_x      = 10,
        .last_pos_y = 5,
        .last_pos_x = 10,
        .map_pos_y  = 1,
        .map_pos_x  = 0
    };

    // Créa ennemi
    CreatureMarine crabe = {
        .id = 1,
        .nom = "Poisson-Scie",
        .points_de_vie_actuels = 13,
        .points_de_vie_max = 25,
        .attaque_minimale = 12,
        .attaque_maximale = 20,
        .defense = 0,
        .vitesse = 0,
        .effet_special = "aucun",
        .est_vivant = 1
    };

    World *world = init_world(map_hauteur, map_largeur); // h, l
    marquer_zone_as_visited(world, plongeur.map_pos_y, plongeur.map_pos_x); // zone depart => visited
    // Pas le top, mais j'ai fais comme ca au debut, et c'est trop tard pour changer.
    // ────────── Boucle de gameplay ──────────
    int running = 1;
    while (running) {

        // Recup zone a afficher => zone dans laquelle est le joueur
        char **screen = world_current_zone(world, &plongeur);

        // Affichage
        full_screen(world, &plongeur, &crabe, screen, info);

        // Récup input user validé selon l'écran courant
        char cmd = prompt_for_command(world, &plongeur, screen_status);
        if (cmd == '\0') break; // fin d'entrée

        switch (screen_status) {

            // ── Exploration ─────────────────────────────────────────────
            case 0: {
                if (cmd == 'C' || cmd == 'c') {
                    screen_status = 2; // Carte
                }
                else if (cmd == 'I' || cmd == 'i') {
                    screen_status = 3; // Inventaire
                }
                else if (cmd == 'D' || cmd == 'd') {
                    demander_player_for_coords(screen, &plongeur, world);
                    screen_status = 0;
                }
                else if (cmd == 'S' || cmd == 's') {
                    printf("Sauvegarde\n");
                }
                else if (cmd == 'E') {
                    screen_status = 1;
                }
                break;
            }

            // ── Combat ─────────────────────────────────────────────────
            case 1: {
                if (cmd == 'Q') {
                    screen_status = 0;
                }
                // (A,B,C,I,F) à implémenter
                break;
            }

            // ── Carte ──────────────────────────────────────────────────
            case 2: {
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }

            // ── Inventaire ─────────────────────────────────────────────
            case 3: {
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }

            // ── Trésor / autres écrans ────────────────────────────────
            case 4: {
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0;
                }
                break;
            }

            default:
                screen_status = 0;
                break;
        }

        printf("\n"); // petite séparation
    }

    free_world(world);
    return 0;
}
