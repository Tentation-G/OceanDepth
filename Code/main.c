#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "save/save.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "globals/globals.h"
#include "types/types.h"
#include "world/world.h"
#include "render/render.h"
#include "input/input.h"
#include "player/player.h"

#include "creature/creature.h"
#include "combat/combat.h"
#include "inventaire/inventaire.h"

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
        .map_pos_x  = 0,

    };

    init_player_inventory(&plongeur);


    World *world = init_world(map_hauteur, map_largeur); // h, l
    marquer_zone_as_visited(world, plongeur.map_pos_y, plongeur.map_pos_x); // zone depart => visited
    // Pas le top, mais j'ai fais comme ca au debut, et c'est trop tard pour changer.
    // ────────── Boucle de gameplay ──────────
    int running = 1;
    while (running) {

        // Recup zone a afficher => zone dans laquelle est le joueur
        char **screen = world_current_zone(world, &plongeur);

        // Affichage
        full_screen(world, &plongeur, g_creatures_en_combat, screen, info);

        // Récup input user validé selon l'écran courant
        char cmd = prompt_for_command(world, &plongeur, screen_status);
        if (cmd == '\0') break; // fin d'entrée

        // if (screen_status != 3) {
        //     previous_screen_status = screen_status;
        // }


        switch (screen_status) {

            // ── Exploration ─────────────────────────────────────────────
            case 0: {
                if (cmd == 'C' || cmd == 'c') {
                    screen_status = 20; // Carte
                }
                else if (cmd == 'I' || cmd == 'i') {
                    previous_screen_status = 0;
                    screen_status = 3; // Inventaire
                }
                else if (cmd == 'D' || cmd == 'd') {
                    demander_player_for_coords(screen, &plongeur, world);
                    
                }
                else if (cmd == 'S' || cmd == 's') {
                    info = "Sauvegarde";
                    sauvegarder(&plongeur);
                }
                else if (cmd == 'E') {
                    screen_status = 1;
                    
                }
                break;
            }

            // ── Combat ─────────────────────────────────────────────────
            case 1: {
                // Logique combat
                previous_screen_status = screen_status;
                gerer_tour_combat(&plongeur, cmd, screen);
                break;
            }

            // ── Carte ──────────────────────────────────────────────────
            case 20: { // ecran selection
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                if (cmd == '1') {
                    screen_status = 21; // go Carte 1
                }
                if (cmd == '2') {
                    screen_status = 22; // go Carte 2
                }
                if (cmd == '3') {
                    screen_status = 23; // go Carte 3
                }
                if (cmd == '4') {
                    screen_status = 24; // go Carte 4
                }
                break;
            }
            case 21: { // Carte 1
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }
            case 22: { // Carte 2
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }
            case 23: { // Carte 3
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }
            case 24: { // Carte 4
                if (cmd == 'R' || cmd == 'r') {
                    screen_status = 20; // retour Ecran selection carte
                }
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0; // retour exploration
                }
                break;
            }

            // ── Inventaire ─────────────────────────────────────────────
            case 3: {
                gerer_inventaire(cmd, &plongeur);
                break;
            }

            // ── Trésor / autres écrans ────────────────────────────────
            case 4: {
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 0;
                }
                break;
            }case 5:{
            
                if (cmd == 'Q' || cmd == 'q') {
                    screen_status = 1;  // Retour combat
                    info = "Retour au combat";
                }
                if (cmd >= '1' && cmd <= '4') {
                    int choix_comp = cmd - '0'; // ici pour convertire char ver int (a changer apres)
                    appliquer_competence(&plongeur, choix_comp);
                    screen_status = 1;  // Retour combat
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
    if (g_creatures_en_combat) free(g_creatures_en_combat);
    return 0;
}
